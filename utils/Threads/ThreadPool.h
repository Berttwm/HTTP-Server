#ifndef ThreadPool_H
#define ThreadPool_H

#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <iostream>
#include <condition_variable>

class ThreadPool 
{
private:
    int num_threads;
    std::vector<std::thread> thread_pool;
    std::queue<std::function<void()>> queue; // queue for storing jobs
    std::condition_variable condition;
    std::mutex queue_mutex;

    bool m_accept_functions = true;
public:
    ThreadPool();
    void start_thread();
    void add_job(std::function<void()> New_Job);
    void terminate();
};

#endif // ThreadPool_H
