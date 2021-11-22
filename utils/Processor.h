#ifndef _PROCESSOR_H
#define _PROCESSOR_H

#include <fstream>

#include "config.hpp"
#include "Cache.h"

class Processor {
private:        
    std::ifstream bm_file;
    int N = 1; // offset size
    int M = 1; // number of sets

    Cache* cache;
    Bus* bus;
    GlobalLock* gl;
    uint32_t instr;
    uint32_t val;
    int index_test;
    int PID;

    // Statistics
    long total_cycle = 0;
    int compute_cycle = 0;
    int num_mem_instr = 0; // Number of load/store instructions
    int idle_cycle = 0;

public:
    void initialize(int index, protocol prot, benchmark bm, int cache_size, int associativity, int block_size, int PID, Bus *bus, GlobalLock *gl);
    Cache* get_cache();
    void run();
    long get_total_cycle();
    int get_compute_cycle();
    int get_num_mem_instr();
    int get_idle_cycle();
    int get_num_cache_miss();
    int get_num_data_traffic();
    int get_num_update();
    int get_num_access_private();
    int get_num_access_shared();

};

#endif