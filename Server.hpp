#ifndef WOCKYTALK_SERVER_HPP
#define WOCKYTALK_SERVER_HPP
#include <mutex>
#include <vector>

class Server
{
private:
    unsigned short port;
    int serverFd{};

public:
    std::vector<int> clients;
    std::mutex clientsMutex;

    explicit Server(unsigned short port) : port(port) {}

    void start();

    void broadcast(int senderSocket, const char *message, unsigned long size);
};
#endif//WOCKYTALK_SERVER_HPP