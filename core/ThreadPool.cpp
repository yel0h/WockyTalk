#include "ThreadPool.hpp"

ThreadPool::ThreadPool(unsigned long numThreads)
{
    for (unsigned long i = 0; i < numThreads; ++i)
    {
        workers.emplace_back([this]()
                             {
                                 while (true)
                                 {
                                     std::function<void()> task;

                                     {
                                         std::unique_lock<std::mutex> lock(queueMutex);
                                         cv.wait(lock, [this]() { return stop || !tasks.empty(); });
                                         if (stop && tasks.empty())
                                         {
                                             return;
                                         }

                                         task = std::move(tasks.front());
                                         tasks.pop();
                                     }

                                     task();
                                 }
                             });
    }
}

void ThreadPool::enqueue(std::function<void()> task)
{
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        tasks.push(std::move(task));
    }

    cv.notify_one();
}

ThreadPool::~ThreadPool()
{
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        stop = true;
    }

    cv.notify_all();
    for (std::thread &worker : workers)
    {
        worker.join();
    }
}