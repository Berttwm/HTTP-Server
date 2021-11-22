#include "Processor.h"

#include <sstream>
#include <string>

void Processor::initialize(int index, protocol prot, benchmark bm, int cache_size, int associativity, int block_size, int PID, Bus *bus, GlobalLock *gl) {
    this->bus = bus;
    this->gl = gl;
    this->PID = PID;
    switch (prot) {
    case protocol::MESI:
        cache = new Cache_MESI();
        cache->set_params(cache_size, associativity, block_size, PID, bus, gl);
        break;
    
    case protocol::Dragon:
        cache = new Cache_Dragon();
        cache->set_params(cache_size, associativity, block_size, PID, bus, gl);
        break;

    case protocol::MOESI:
        cache = new Cache_MOESI();
        cache->set_params(cache_size, associativity, block_size, PID, bus, gl);
        break;

    default:
        std::cout << "[ERROR] Protocol type wrong (in cache)." << std::endl; 
        return;   
    }
    
    std::string path;
    switch(bm) {
        case benchmark::blackscholes:
            path = "blackscholes_four/blackscholes_";
            break;
        case benchmark::bodytrack:
            path = "bodytrack_four/bodytrack_";
            break;
        case benchmark::fluidanimate:
            path = "fluidanimate_four/fluidanimate_";
            break;  
    }
    path += std::to_string(index) + ".data";
    index_test = index;
    bm_file.open(path, std::ifstream::in);
    // std::cout << bm_file.is_open() << std::endl;

    if (cache_size % block_size != 0) {
        std::cout << "[ERROR] Cache size must be divisible by block size." << std::endl;
        return;
    } else if ((cache_size/block_size) % associativity != 0) {
        std::cout << "[ERROR] The total number of cache block must be divisible by associativity." << std::endl;
        return;
    }
    M = (cache_size / block_size) / associativity;
    N = block_size;

    return;
}

Cache* Processor::get_cache() {
    return cache;
}

long Processor::get_total_cycle() {
    return total_cycle;
}

int Processor::get_compute_cycle() {
    return compute_cycle;
}

int Processor::get_num_mem_instr() {
    return num_mem_instr;
}

int Processor::get_idle_cycle() {
    return idle_cycle;
}

int Processor::get_num_cache_miss() {
    return cache->num_cache_miss;
}

int Processor::get_num_data_traffic() {
    return cache->num_data_traffic;
}

int Processor::get_num_update() {
    return cache->num_update;
}

int Processor::get_num_access_private() {
    return cache->num_access_private;
}

int Processor::get_num_access_shared() {
    return cache->num_access_shared;
}

void Processor::run() {
    std::string str_val;
    while(bm_file >> instr >> str_val) {
        std::stringstream ss;
        ss << std::hex << str_val;
        ss >> val;
            
        // std::cout << "[" << index_test << "]  " << instr << " | " << val << std::endl;
        if (instr == 0 || instr == 1) {
            num_mem_instr += 1;
            int set_index = (val / N) % M;
            // std::cout << "[" << set_index << "]" << std::endl;
            int tag = (val / N) / M;
            if (instr == 0) { // read
                idle_cycle += cache->pr_read(set_index, tag);      
            } else { // write
                idle_cycle += cache->pr_write(set_index, tag);
            }
            total_cycle += idle_cycle;
        } else {
            if (instr != 2) {
                std::cout << "[ERROR] Instr index value goes out of range." << std::endl;
                return;
            }
            compute_cycle += val;
            total_cycle += val;
        }
    }     
    return; 
}