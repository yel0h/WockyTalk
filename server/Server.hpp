#ifndef WOCKYTALK_SERVER_HPP
#define WOCKYTALK_SERVER_HPP
#include "../core/MessageQueue.hpp"
#include "../core/ThreadPool.hpp"
#include <mutex>
#include <vector>

class Server
{
private:
    unsigned short port;
    int serverFd{};
    ThreadPool pool{4};

public:
    std::vector<int> clients;
    std::mutex clientsMutex;
    MessageQueue messageQueue;

    explicit Server(unsigned short port) : port(port) {}

    void start();

    void broadcast(int senderSocket, const char *message, unsigned long size);

    void acceptLoop();
};
#endif//WOCKYTALK_SERVER_HPP