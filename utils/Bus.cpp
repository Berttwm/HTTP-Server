#include "Bus.h"
#include "Cache.h"



void Bus::init_bus(int cache_size, int associativity, int block_size, GlobalLock *gl){
    this->num_blocks = (cache_size / block_size) / associativity;
    this->associativity = associativity;
    this->offset = block_size;
    this->gl = gl;
    std::cout << "[INFO] Bus initialized" << std::endl;

}

void Bus::init_cache(Cache *c0, Cache *c1, Cache *c2, Cache *c3) {
    cache_list.push_back(c0);
    cache_list.push_back(c1);
    cache_list.push_back(c2);
    cache_list.push_back(c3);
}
/* 
***************************************************************
MESI Bus Protocol APIs
***************************************************************
*/
int Bus_MESI::BusRd(int PID, int i_set, int tag, Cache *cache) {
    int status = status_MESI::I;
    for (int i = 0; i < num_cores; i++) {
        if (i == PID) continue;

        int curr_status = cache_list[i]->get_status_cacheline(i_set, tag);
        if (curr_status != status_MESI::I) {

            // if curr_state is M, M->S, write back to memory
            if (curr_status == status_MESI::M) {
                cache_list[i]->num_data_traffic += 1;
            }
            cache_list[i]->set_status_cacheline(i_set, tag, status_MESI::S, op_type::read_op);
            status = curr_status;
            break;
        }
        if (status != status_MESI::I) break;
    }
    return status;
}

int Bus_MESI::BusUpd(int PID, int i_set, int tag, Cache *cache) {
    // number of cycles for BusRdX should be 0
    int num_invalidation = 0;  
    for(int i = 0; i < num_cores; i++) {
        if (i == PID) continue;

        int curr_status = cache_list[i]->get_status_cacheline(i_set, tag);
        if (curr_status != status_MESI::I) {
            // Invalidate all cache Lines
            num_invalidation += 1;
            cache_list[i]->set_status_cacheline(i_set, tag, status_MESI::I, op_type::write_op);
        }
    }
    return num_invalidation; // result not used for MESI bus
}
/* 
***************************************************************
Dragon Bus Protocol APIs
***************************************************************
*/
int Bus_Dragon::BusRd(int PID, int i_set, int tag, Cache *cache) {
    int status = status_Dragon::not_found;
    for (int i = 0; i < num_cores; i++) {
        if (i == PID) continue;

        int curr_status = cache_list[i]->get_status_cacheline(i_set, tag);
        if (curr_status != status_Dragon::not_found) {
            // change all exclusive states to shared states
            if (curr_status == status_Dragon::D || curr_status == status_Dragon::Sm) {
                cache_list[i]->set_status_cacheline(i_set, tag, status_Dragon::Sm, op_type::read_op);
            } else if(curr_status == status_Dragon::E_DRAGON || curr_status == status_Dragon::Sc) {
                cache_list[i]->set_status_cacheline(i_set, tag, status_Dragon::Sc, op_type::read_op);
            }
            status = curr_status;
            break;
        }
        if (status != status_Dragon::not_found) break;
    }
    return status;
}
int Bus_Dragon::BusUpd(int PID, int i_set, int tag, Cache *cache) { 
    int num_update = 0;
    int curr_status;
    for (int i = 0; i < num_cores; i++) {
        if (i == PID) continue;

        curr_status = cache_list[i]->get_status_cacheline(i_set, tag);
        if (curr_status != status_Dragon::not_found) {
            // Overwrite the target cache's cache line
            if (curr_status == status_Dragon::Sm)
                cache_list[i]->set_status_cacheline(i_set, tag, status_Dragon::Sc, op_type::write_op);
            num_update += 1; 
        }
        if (curr_status != status_Dragon::not_found) break;
    }
    return num_update;
}
/* 
***************************************************************
MOESI Bus Protocol APIs
***************************************************************
*/
int Bus_MOESI::BusRd(int PID, int i_set, int tag, Cache *cache) {
    int status = status_MOESI::I_MO;
    int o_status = status_MOESI::I_MO;
    for (int i = 0; i < num_cores; i++) {
        if (i == PID) continue;

        int curr_status = cache_list[i]->get_status_cacheline(i_set, tag);
        if (curr_status != status_MOESI::I_MO) {
            if(curr_status == status_MOESI::M_MO || status_MOESI::O_MO) {
                o_status = status_MOESI::M_MO;
                cache_list[i]->set_status_cacheline(i_set, tag, status_MOESI::O_MO, op_type::read_op);
            }
            if(curr_status == status_MOESI::E_MO || status_MOESI::S_MO) {
                cache_list[i]->set_status_cacheline(i_set, tag, status_MOESI::S_MO, op_type::read_op);
            }

            if(curr_status != status_MOESI::I_MO) status = curr_status;
        }
    }
    if(o_status != status_MOESI::I_MO) return o_status;
    return status;
}

int Bus_MOESI::BusUpd(int PID, int i_set, int tag, Cache *cache) {
    int num_invalidation = 0;  
    for(int i = 0; i < num_cores; i++) {
        if (i == PID) continue;

        int curr_status = cache_list[i]->get_status_cacheline(i_set, tag);
        if (curr_status != status_MOESI::I_MO) {
            // Invalidate all other cache Lines
            num_invalidation += 1;
            cache_list[i]->set_status_cacheline(i_set, tag, status_MOESI::I_MO, op_type::write_op);
        }
    }
    return num_invalidation; 
}