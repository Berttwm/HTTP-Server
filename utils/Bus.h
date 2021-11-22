#ifndef _BUS_H
#define _BUS_H

#include <vector>

#include "GlobalLock.hpp"
#include "config.hpp"


class Cache;

class Bus {
private:

public:
    typedef int pid;
    const int num_cores = 4;
    int num_blocks; 
    int associativity;
    int offset;
    GlobalLock *gl;
    std::vector<Cache*> cache_list;

    // // latest_memory has the view of the latest memory written - do we need this?
    // std::vector<std::vector<int>> latest_memory;
    void init_bus(int cache_size, int associativity, int block_size, GlobalLock *gl);
    void init_cache(Cache *c0, Cache *c1, Cache *c2, Cache *c3);

    // Cache to Bus transactions API
    virtual int BusRd(int PID, int i_set, int tag, Cache *cache) = 0;
    virtual int BusUpd(int PID, int i_set, int tag, Cache *cache) = 0; // BusRdX for Bus_MESI, BusUpdate for Bus_Dragon
};

class Bus_MESI : public Bus {
    int BusRd(int PID, int i_set, int tag, Cache *cache);
    int BusUpd(int PID, int i_set, int tag, Cache *cache);
};

class Bus_Dragon : public Bus {
    int BusRd(int PID, int i_set, int tag, Cache *cache);
    int BusUpd(int PID, int i_set, int tag, Cache *cache);
};

class Bus_MOESI : public Bus {
    int BusRd(int PID, int i_set, int tag, Cache *cache);
    int BusUpd(int PID, int i_set, int tag, Cache *cache);
};

#endif