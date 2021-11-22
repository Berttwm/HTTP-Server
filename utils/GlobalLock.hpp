#ifndef _GLOBALLOCK_HPP
#define _GLOBALLOCK_HPP
/* Global Lock 
 * List of global locks. Each cache line has global lock (lock entire all N-way sets when calling lock)
 * necessary for lock to be visible to all processes (not just within bus) so that writes and reads are
 * follow Write-Serialization && preserve Program Order property
*/
#include <iostream>
#include <mutex>
#include <vector>

class GlobalLock {
   private:
   public:
    int num_blocks;
    std::vector<std::mutex> mutexes;
    GlobalLock(int cache_size, int associativity, int block_size) {
        /* Step 1: Initialize list of locks 
        * Every set within the cache that the bus reads must be locked from within the cache
        * Example: 64 sets means we need 64 locks, if we access set_idx X, we only acquire set_idx X's lock
        */
        this->num_blocks = (cache_size / block_size) / associativity;
        std::vector<std::mutex> temp_locks_list(this->num_blocks);
        mutexes.swap(temp_locks_list);
        std::cout << "[INFO] GlobalLock initialized with: " << num_blocks << " locks" << std::endl;
    }
    void gl_lock(int idx) {
        if(idx < 0 || idx > this->num_blocks) { // error input
            std::cout << "[ERROR] Global locks: trying to acquire lock that is not within range." << std::endl;
        } else {
            mutexes[idx].lock();
        }
    }
    void gl_unlock(int idx) {
        if(idx < 0 || idx > this->num_blocks) { // error input
            std::cout << "[ERROR] Global locks: trying to release lock that is not within range." << std::endl;
        } else {
            mutexes[idx].unlock();

        }
    }
};
#endif // _GLOBALLOCK_HPP
