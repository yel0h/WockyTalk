#ifndef WOCKYTALK_THREADPOOL_HPP
#define WOCKYTALK_THREADPOOL_HPP
#include <condition_variable>
#include <functional>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool
{
private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable cv;
    bool stop = false;

public:
    explicit ThreadPool(unsigned long numThreads);

    void enqueue(std::function<void()> task);

    ~ThreadPool();
};
#endif//WOCKYTALK_THREADPOOL_HPP