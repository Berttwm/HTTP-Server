#include "ThreadPool.h"

ThreadPool::ThreadPool()
{
    std::cout << "==== Initializing ThreadPool... ====" << std::endl;
    num_threads = num_threads = std::thread::hardware_concurrency();
    for(int i = 0; i < num_threads; i++)
    {
        thread_pool.push_back(std::thread(&ThreadPool::start_thread));
    }
    std::cout << "==== Finished Initializing (number of threads = " << num_threads << ") ThreadPool ====" << std::endl;
}

void ThreadPool::start_thread()
{
    std::function<void()> job;
    while (true)
    {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            condition.wait(lock, [this]() {return !queue.empty() || !m_accept_functions; });
            if (!m_accept_functions && queue.empty())
            {
                //lock will be release automatically.
                //finish the thread loop and let it join in the main thread.
                return;
            }
            job = queue.front();
            queue.pop();
            //release the lock
        }
        job();
    }
}

void ThreadPool::add_job(std::function<void()> New_Job)
{
    std::unique_lock<std::mutex> lock(queue_mutex);
    queue.push(New_Job);
    // when we send the notification immediately, the consumer will try to get the lock , so unlock asap
    lock.unlock();
    condition.notify_one();
}

void ThreadPool::terminate()
{
    std::unique_lock<std::mutex> lock(queue_mutex);
    m_accept_functions = false;
    lock.unlock();
    // when we send the notification immediately, the consumer will try to get the lock , so unlock asap
    condition.notify_all();
    //notify all waiting threads.

    for(std::thread &th : thread_pool)
    {
        th.join();
    }
    thread_pool.clear();
    // stopped = true;
}