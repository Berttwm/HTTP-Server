#ifndef _CACHE_H
#define _CACHE_H

#include <vector>

#include "GlobalLock.hpp"
#include "config.hpp"

class Bus;

class Cache {
public:
    
    int PID;
    int num_sets;
    int num_ways;
    int block_size = 32;
    Bus *bus;
    GlobalLock *gl;
    std::vector<std::vector<std::vector<int>>> dummy_cache;

    // Statistics
    int num_cache_miss = 0;
    int num_data_traffic = 0;
    int num_update = 0; // Number of invalidations or updates on the bus
    int num_access_private = 0; // Access to modified state is private
    int num_access_shared = 0; // Access to shared state is shared

    // Construct a dummy cache with shape: associativity(num_ways), num of cache set, 2
    // 2: index 0 for states, index 1 for tag
    void set_params(int cache_size, int associativity, int blk_size, int PID, Bus *bus, GlobalLock *gl);

    /*
    ** To maintain LRU replacement policy, old data in the given cache set are shifted to left
    */
    int shift_cacheline_left_until(int i_set, int pos, bool for_new_element);

    virtual int pr_read(int i_set, int tag) = 0;
    virtual int pr_write(int i_set, int tag) = 0;
    /* Cache to bus transactions */

    virtual int get_status_cacheline(int i_set, int tag) = 0;
    virtual int set_status_cacheline(int i_set, int tag, int status, int op) = 0;
        
};

class Cache_MESI : public Cache {
private:
public:
    int pr_read(int i_set, int tag);
    int pr_write(int i_set, int tag);
    int get_status_cacheline(int i_set, int tag);
    int set_status_cacheline(int i_set, int tag, int status, int op);

};

class Cache_Dragon : public Cache {
private:
public:
    int pr_read(int i_set, int tag);
    int pr_write(int i_set, int tag);
    int get_status_cacheline(int i_set, int tag);
    int set_status_cacheline(int i_set, int tag, int status, int op);
};

class Cache_MOESI : public Cache {
private:
public:
    int pr_read(int i_set, int tag);
    int pr_write(int i_set, int tag);
    int get_status_cacheline(int i_set, int tag);
    int set_status_cacheline(int i_set, int tag, int status, int op);
};

#endif