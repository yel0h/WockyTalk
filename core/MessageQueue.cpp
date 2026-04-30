#include "MessageQueue.hpp"

void MessageQueue::push(Message msg)
{
    {
        std::lock_guard<std::mutex> lock(mutex);
        queue.push(std::move(msg));
    }

    cv.notify_one();
}

Message MessageQueue::pop()
{
    std::unique_lock<std::mutex> lock(mutex);
    cv.wait(lock, [this]() { return !queue.empty(); });
    Message msg = std::move(queue.front());
    queue.pop();
    return msg;
}