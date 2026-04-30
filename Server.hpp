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
};
#endif//WOCKYTALK_SERVER_HPP