#ifndef WOCKYTALK_MESSAGEQUEUE_HPP
#define WOCKYTALK_MESSAGEQUEUE_HPP
#include "../common/Message.hpp"
#include <condition_variable>
#include <mutex>
#include <queue>

class MessageQueue
{
private:
    std::queue<Message> queue;
    std::mutex mutex;
    std::condition_variable cv;

public:
    void push(Message msg);

    Message pop();
};
#endif//WOCKYTALK_MESSAGEQUEUE_HPP