#include "Cache.h"
#include "Bus.h"
#include "config.hpp"


void Cache::set_params (int cache_size, int associativity, int blk_size, int PID, Bus *bus, GlobalLock *gl) {
    this->PID = PID;
    num_ways = associativity;
    num_sets = (cache_size / blk_size) / associativity;
    block_size = blk_size;
    std::vector<int> temp(2, 0);
    std::vector<std::vector<int>> temp2;
    for (int i = 0; i < num_sets; i++) {
        temp2.push_back(temp);
    }
    for (int i = 0; i < associativity; i++) {
        dummy_cache.push_back(temp2);
    }
    
    this->bus = bus;
    this->gl = gl;
}

/*
** To maintain LRU replacement policy, old data in the given cache set are shifted to left till pos
*  pos = 0 for read-miss
*  Output: return the cycle taken, once eviction occurs, the shifting takes 100 cycles
*/
int Cache::shift_cacheline_left_until(int i_set, int pos, bool for_new_element) {
    bool flush = false;
    // If flush back occurs
    if (pos == 0 && (dummy_cache[0][i_set][cache_line::status] == status_MESI::M || 
                    dummy_cache[0][i_set][cache_line::status] == status_Dragon::D ||
                    dummy_cache[0][i_set][cache_line::status] == status_Dragon::Sm ||
                    dummy_cache[0][i_set][cache_line::status] == status_MOESI::M_MO || 
                    dummy_cache[0][i_set][cache_line::status] == status_MOESI::O_MO 
                    ) && for_new_element) {
        num_data_traffic += 1;
        flush = true;
    }
    for (int i = pos; i < num_ways-1; i++) { 
        dummy_cache[i][i_set] = dummy_cache[i+1][i_set];
    }

    return flush? 100 : 0;
}
/* 
***************************************************************
MESI Cache Protocol APIs
***************************************************************
*/
int Cache_MESI::pr_read(int i_set, int tag) {
    int curr_op_cycle = 1;
    gl->gl_lock(i_set);
    for (int i = 0; i < num_ways; i++) {
        // Read hit
        if ((dummy_cache[i][i_set][cache_line::status] != status_MESI::I) && (dummy_cache[i][i_set][cache_line::tag] == tag)) {
            // Update LRU Policy - Read Hit
            std::vector<int> temp = dummy_cache[i][i_set];
            shift_cacheline_left_until(i_set,i,false);
            dummy_cache[num_ways-1][i_set] = temp; // set last line to temp 
            // check the type of access
            switch (dummy_cache[num_ways-1][i_set][cache_line::status]) {
            case status_MESI::M:
            case status_MESI::E_MESI:
                num_access_private += 1;
                break;           
            case status_MESI::S:
                num_access_shared += 1;
                break;
            }

            gl->gl_unlock(i_set);
            return curr_op_cycle;
        }
    }
    // Read miss
    // Update LRU Policy - Read miss
    curr_op_cycle += shift_cacheline_left_until(i_set, 0, true); 
    dummy_cache[num_ways-1][i_set][cache_line::tag] = tag; // set last line to new 
    num_cache_miss += 1;
    num_data_traffic += 1;
    Cache *placeholder;
    if (bus->BusRd(PID, i_set, tag, placeholder) == status_MESI::I) {
        // I -> E
        // Fetching a block from memory to cache takes additional 100 cycles
        num_access_private += 1;
        curr_op_cycle += 100;
        dummy_cache[num_ways-1][i_set][cache_line::status] = status_MESI::E_MESI;
        
    } else {
        // I -> S
        // Fetching a block from other cache to my cache takes additional 2N cycles
        num_access_shared += 1;
        curr_op_cycle += 2 * (block_size/4);
        dummy_cache[num_ways-1][i_set][cache_line::status] = status_MESI::S;
    }
    gl->gl_unlock(i_set);
    return curr_op_cycle; // placeholder
}

int Cache_MESI::pr_write(int i_set, int tag) {
    int curr_op_cycle = 1;
    int curr_update = 0;
    gl->gl_lock(i_set);
    for (int i = 0; i < num_ways; i++) {
        // Write hit
        if ((dummy_cache[i][i_set][cache_line::status] != status_MESI::I) && (dummy_cache[i][i_set][cache_line::tag] == tag)) {

            // 1. Update LRU Policy First - Write Hit
            std::vector<int> temp = dummy_cache[i][i_set];
            shift_cacheline_left_until(i_set,i,false);
            dummy_cache[num_ways-1][i_set] = temp; // set last line to temp 

            // 2. Set status
            switch (dummy_cache[num_ways-1][i_set][cache_line::status]) {
            case status_MESI::M:
                num_access_private += 1;
                break;
            case status_MESI::E_MESI:
                num_access_private += 1;
                dummy_cache[num_ways-1][i_set][cache_line::status] = status_MESI::M;
                break;
            case status_MESI::S:
                num_access_shared += 1;
                dummy_cache[num_ways-1][i_set][cache_line::status] = status_MESI::M;
                Cache *placeholder;

                curr_update = bus->BusUpd(PID, i_set, tag, placeholder);
                num_update += curr_update;
                curr_op_cycle += 2*curr_update;
                break;
            }
            gl->gl_unlock(i_set);
            return curr_op_cycle;
        }
            
    }
    // Write miss policy: Write-back, write-allocate
    // Step 1: read line into cache block
    num_cache_miss += 1;
    num_data_traffic += 1;
    Cache *placeholder;
    if (bus->BusRd(PID, i_set, tag, placeholder) == status_MESI::I) {
        // Fetching a block from memory to cache takes additional 100 cycles
        num_access_private += 1;
        curr_op_cycle += 100;
    } else {
        // Fetching a block from another cache to mine takes 2N cycles
        curr_op_cycle += 2 * (block_size/4);

        curr_update = bus->BusUpd(PID, i_set, tag, placeholder);
        num_update += curr_update;
        num_access_shared += 1;
        // Invalidate the block in each other caches takes 2 
        curr_op_cycle += 2*curr_update;
    }
    // Step 2: Update LRU Policy - Write Miss
    curr_op_cycle += shift_cacheline_left_until(i_set, 0, true); 
    // Step 3: Set last line to new and modified
    dummy_cache[num_ways-1][i_set][cache_line::tag] = tag; 
    dummy_cache[num_ways-1][i_set][cache_line::status] = status_MESI::M; 
    
    gl->gl_unlock(i_set);
    return curr_op_cycle; // placeholder
}

int Cache_MESI::get_status_cacheline(int i_set, int tag) {
    int status = status_MESI::I;
    for (int i = 0; i < num_ways; i++) {
        if (dummy_cache[i][i_set][cache_line::tag] == tag) {
            status = dummy_cache[i][i_set][cache_line::status];
            break;
        }
    }
    // gl->gl_unlock(i_set);
    return status;
}

int Cache_MESI::set_status_cacheline(int i_set, int tag, int status, int op) {
    for (int i = 0; i < num_ways; i++) {
        if (dummy_cache[i][i_set][cache_line::tag] == tag) {
            dummy_cache[i][i_set][cache_line::status] = status;
            // Update LRU policy of other cache if set_status is a write for Dragon protocol
            std::vector<int> temp = dummy_cache[i][i_set];
            shift_cacheline_left_until(i_set,i,false);
            dummy_cache[num_ways-1][i_set] = temp; // set last line to temp
            break;
        }
    }
    return 1; // placeholder
}

/* 
***************************************************************
Dragon Cache Protocol APIs
***************************************************************
*/
int Cache_Dragon::pr_read(int i_set, int tag) {
    int curr_op_cycle = 1;
    gl->gl_lock(i_set);

    for (int i = 0; i < num_ways; i++) {
        // Read hit
        if ((dummy_cache[i][i_set][cache_line::status] != status_Dragon::not_found) && (dummy_cache[i][i_set][cache_line::tag] == tag)) {
            // Update LRU Policy - Read hit
            std::vector<int> temp = dummy_cache[i][i_set];
            shift_cacheline_left_until(i_set,i,false);
            dummy_cache[num_ways-1][i_set] = temp; // set last line to temp

            // update the type of access based on the block status
            switch (dummy_cache[num_ways-1][i_set][cache_line::status]) {
            case status_Dragon::D:
            case status_Dragon::E_DRAGON:
                num_access_private += 1;
                break;
            case status_Dragon::Sm:
            case status_Dragon::Sc:
                num_access_shared += 1;
                break;
            }

            gl->gl_unlock(i_set);
            return curr_op_cycle;
        }
    }
    // Read miss
    num_data_traffic += 1;
    num_cache_miss += 1;
    // Update LRU Policy - Read miss
    curr_op_cycle += shift_cacheline_left_until(i_set, 0, true); 
    dummy_cache[num_ways-1][i_set][cache_line::tag] = tag; // set last line to new 

    Cache *placeholder;
    if (bus->BusRd(PID, i_set, tag, placeholder) == status_Dragon::not_found) {
        // not_found -> E
        // Fetching a block from memory to cache takes additional 100 cycles
        num_access_private += 1;
        curr_op_cycle += 100;
        dummy_cache[num_ways-1][i_set][cache_line::status] = status_Dragon::E_DRAGON;
    } else {
        // not_found -> Sc
        num_access_shared += 1;
        // Fetching a block from other cache to my cache takes additional 2N cycles
        curr_op_cycle += 2 * (block_size/4);
        dummy_cache[num_ways-1][i_set][cache_line::status] = status_Dragon::Sc;
    }
    gl->gl_unlock(i_set);
    return curr_op_cycle; // placeholder
}

int Cache_Dragon::pr_write(int i_set, int tag) {
    int curr_op_cycle = 1;
    int curr_update = 0;
    Cache *placeholder;
    gl->gl_lock(i_set);

    for (int i = 0; i < num_ways; i++) {
        // Write hit
        if ((dummy_cache[i][i_set][cache_line::status] != status_Dragon::not_found) && (dummy_cache[i][i_set][cache_line::tag] == tag)) {
            // 1. Shift before setting status
            std::vector<int> temp = dummy_cache[i][i_set];
            shift_cacheline_left_until(i_set,i,false);
            dummy_cache[num_ways-1][i_set] = temp; // set last line to temp
            
            // 2. Set status
            switch (dummy_cache[num_ways-1][i_set][cache_line::status]) {
            case status_Dragon::E_DRAGON:
                // TODO
                dummy_cache[num_ways-1][i_set][cache_line::status] = status_Dragon::D;
            case status_Dragon::D:
                // accessing E or D are both private data access
                num_access_private += 1;
                break;
            case status_Dragon::Sc:
                // Same as Sm
            case status_Dragon::Sm:
                num_access_shared += 1;
                if (bus->BusRd(PID, i_set, tag, placeholder) == status_Dragon::not_found) {
                    // not found in other cache
                    dummy_cache[num_ways-1][i_set][cache_line::status] = status_Dragon::D;
                    curr_op_cycle += 100;
                } else {
                    // found in other cache
                    // Each write to another cache block incurs 2N cycles
                    curr_update = bus->BusUpd(PID, i_set, tag, placeholder);
                    dummy_cache[num_ways-1][i_set][cache_line::status] = status_Dragon::Sm;

                    // Update counters
                    num_update += curr_update;
                    num_data_traffic += curr_update;
                    curr_op_cycle += curr_update * 2 * (block_size/4);
                }
                break;

            }
            
            gl->gl_unlock(i_set);
            return curr_op_cycle;
        }
        
    }
    // Write-Miss 
    num_cache_miss += 1;
    // It takes 1 data traffic to fetch from either memory or other caches
    num_data_traffic += 1;
    // Step 1: Update LRU Policy - Write Hit
    curr_op_cycle += shift_cacheline_left_until(i_set, 0, true); 
    if (bus->BusRd(PID, i_set, tag, placeholder) == status_Dragon::not_found) {
        // Fetching a block from memory to cache 
        num_access_private += 1;
        // Step 2: Set last line to new and modified
        dummy_cache[num_ways-1][i_set][cache_line::tag] = tag; 
        dummy_cache[num_ways-1][i_set][cache_line::status] = status_Dragon::D; 

        curr_op_cycle += 100; // read from memory operation
    } else {
        // Fetching a block from another cache to mine takes 2N cycles
        num_access_private += 1;
        // Step 2: Set last line to new and modified
        dummy_cache[num_ways-1][i_set][cache_line::tag] = tag; 
        dummy_cache[num_ways-1][i_set][cache_line::status] = status_Dragon::Sm; 

        curr_op_cycle += 2*(block_size/4); // read from other cache lines first

        // Step 3: then update all included cache lines
        // For each BusUpd to other caches, it takes 2N cycles 
        // where N = num of word and 1 word == 4 bytes
        curr_update = bus->BusUpd(PID, i_set, tag, placeholder);
        num_update += curr_update;
        num_data_traffic += curr_update;
        curr_op_cycle += curr_update * 2 * (block_size/4);
    }

    gl->gl_unlock(i_set);
    return curr_op_cycle; // placeholder    // TODO: handle the case when the block not in cache
}

int Cache_Dragon::get_status_cacheline(int i_set, int tag) {
    int status = status_Dragon::not_found;
    for (int i = 0; i < num_ways; i++) {
        if (dummy_cache[i][i_set][cache_line::tag] == tag) {
            status = dummy_cache[i][i_set][cache_line::status];
            break;
        }
    }
    return status;
}
    
int Cache_Dragon::set_status_cacheline(int i_set, int tag, int status, int op) {

    for (int i = 0; i < num_ways; i++) {
        if (dummy_cache[i][i_set][cache_line::tag] == tag) {
            dummy_cache[i][i_set][cache_line::status] = status;
            // Update LRU policy of other cache if set_status is a write for Dragon protocol
            std::vector<int> temp = dummy_cache[i][i_set];
            shift_cacheline_left_until(i_set,i, false);
            dummy_cache[num_ways-1][i_set] = temp; // set last line to temp
            break;
        }
    }
    return 1; // placeholder
}
/* 
***************************************************************
MOESI Cache Protocol APIs - Extension
***************************************************************
*/
int Cache_MOESI::pr_read(int i_set, int tag) {
    int curr_op_cycle = 1;
    gl->gl_lock(i_set);
    for (int i = 0; i < num_ways; i++) {
        // Read hit
        if ((dummy_cache[i][i_set][cache_line::status] != status_MOESI::I_MO) && (dummy_cache[i][i_set][cache_line::tag] == tag)) {
            // Update LRU Policy - Read Hit
            std::vector<int> temp = dummy_cache[i][i_set];
            shift_cacheline_left_until(i_set,i, false);
            dummy_cache[num_ways-1][i_set] = temp; // set last line to temp 
            // check the type of access
            switch (dummy_cache[i][i_set][cache_line::status]) {
            case status_MOESI::M_MO:
            case status_MOESI::E_MO:
                num_access_private += 1;
                break; 
            case status_MOESI::O_MO:        
            case status_MOESI::S_MO:
                num_access_shared += 1;
                break;
            }

            gl->gl_unlock(i_set);
            return curr_op_cycle;
        }
    }
    // Read miss
    // Update LRU Policy - Read miss
    curr_op_cycle += shift_cacheline_left_until(i_set, 0, true); 
    dummy_cache[num_ways-1][i_set][cache_line::tag] = tag; // set last line to new 
    num_cache_miss += 1;
    num_data_traffic += 1;
    Cache *placeholder;
    int curr_status = bus->BusRd(PID, i_set, tag, placeholder);
    if (curr_status == status_MOESI::I_MO) {
        // not found in any cache block
        // Fetching a block from memory to cache takes additional 100 cycles
        num_access_private += 1;
        curr_op_cycle += 100;
        dummy_cache[num_ways-1][i_set][cache_line::status] = status_MOESI::E_MO;
    } else {
        // Another cache is the owner - This cache does not inherit ownership (O)
        //      The other cache remains (O->O)
        // Fetching a block from other cache to my cache takes additional 2N cycles
        num_access_shared += 1;
        curr_op_cycle += 2 * (block_size/4);
        dummy_cache[num_ways-1][i_set][cache_line::status] = status_MOESI::S_MO;
    }
    gl->gl_unlock(i_set);
    return curr_op_cycle; // placeholder
}

int Cache_MOESI::pr_write(int i_set, int tag) {
    int curr_op_cycle = 1;
    int curr_update = 0;
    gl->gl_lock(i_set);
    for (int i = 0; i < num_ways; i++) {
        // Write hit
        if ((dummy_cache[i][i_set][cache_line::status] != status_MOESI::I_MO) && (dummy_cache[i][i_set][cache_line::tag] == tag)) {

            // 1. Update LRU Policy First - Write Hit
            std::vector<int> temp = dummy_cache[i][i_set];
            shift_cacheline_left_until(i_set,i, false);
            dummy_cache[num_ways-1][i_set] = temp; // set last line to temp 

            // 2. Set status
            switch (dummy_cache[num_ways-1][i_set][cache_line::status]) {
            case status_MOESI::M_MO:
                num_access_private += 1;
                break;
            case status_MOESI::O_MO:
                // When this cache changes data on the cache line in O state, 
                // it must broadcast those changes to all other caches sharing the line  
                num_access_shared += 1;
                dummy_cache[num_ways-1][i_set][cache_line::status] = status_MOESI::M_MO;
                Cache *placeholder;
                curr_update = bus->BusUpd(PID, i_set, tag, placeholder);
                num_update += curr_update;
                // Sending a word from one cache to another takes 2 cycles
                curr_op_cycle += curr_update * 2;
                break;
            case status_MOESI::E_MO:
                num_access_private += 1;
                dummy_cache[num_ways-1][i_set][cache_line::status] = status_MOESI::M_MO;
                break;
            case status_MOESI::S_MO:
                num_access_shared += 1;
                dummy_cache[num_ways-1][i_set][cache_line::status] = status_MOESI::M_MO;
                break;
            }
            gl->gl_unlock(i_set);
            return curr_op_cycle;
        }
            
    }
    // Write miss policy: Write-back, write-allocate
    num_cache_miss += 1;
    num_data_traffic += 1;
    Cache *placeholder;
    int curr_status = bus->BusRd(PID, i_set, tag, placeholder); // can only be I_MO, O_MO or S_MO
    if (curr_status == status_MOESI::I_MO) {
        // Fetching a block from memory to cache takes additional 100 cycles
        num_access_private += 1;
        curr_op_cycle += 100;
    } else { // == status_MOESI::S_MO, O_MO
        // Fetching a block from another cache to mine takes 2N cycles
        curr_op_cycle += 2 * (block_size/4);

        curr_update = bus->BusUpd(PID, i_set, tag, placeholder);
        num_update += curr_update;
        num_access_shared += 1;

        curr_op_cycle += 2*curr_update;
    }
    // Step 2: Update LRU Policy - Write Miss
    curr_op_cycle += shift_cacheline_left_until(i_set, 0, true); 
    // Step 3: Set last line to new and modified
    dummy_cache[num_ways-1][i_set][cache_line::tag] = tag; 
    dummy_cache[num_ways-1][i_set][cache_line::status] = status_MESI::M; 
    
    gl->gl_unlock(i_set);
    return curr_op_cycle; // placeholder
}

int Cache_MOESI::get_status_cacheline(int i_set, int tag) {
    int status = status_MOESI::I_MO;
    for (int i = 0; i < num_ways; i++) {
        if (dummy_cache[i][i_set][cache_line::tag] == tag) {
            status = dummy_cache[i][i_set][cache_line::status];
            break;
        }
    }
    return status;
}

int Cache_MOESI::set_status_cacheline(int i_set, int tag, int status, int op) {
    for (int i = 0; i < num_ways; i++) {
        if (dummy_cache[i][i_set][cache_line::tag] == tag) {
            dummy_cache[i][i_set][cache_line::status] = status;
            // Update LRU policy of other cache if set_status is a write for Dragon protocol
            std::vector<int> temp = dummy_cache[i][i_set];
            shift_cacheline_left_until(i_set,i, false);
            dummy_cache[num_ways-1][i_set] = temp; // set last line to temp
            break;
        }
    }
    return 1; // placeholder
}