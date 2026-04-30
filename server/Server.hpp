#ifndef WOCKYTALK_SERVER_HPP
#define WOCKYTALK_SERVER_HPP
#include "../core/MessageQueue.hpp"
#include <mutex>
#include <vector>

struct Client
{
    int socket;
    int id;
    std::string name;
};

class Server
{
private:
    unsigned short port;
    int serverFd{};
    std::atomic<int> nextClientId{1};

public:
    std::vector<Client> clients;
    std::mutex clientsMutex;
    MessageQueue messageQueue;

    explicit Server(unsigned short port) : port(port) {}

    void start();

    void broadcast(int senderSocket, const char *message, unsigned long size);

    void acceptLoop();
};
#endif//WOCKYTALK_SERVER_HPP