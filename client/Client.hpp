#ifndef WOCKYTALK_CLIENT_HPP
#define WOCKYTALK_CLIENT_HPP
#include <string>

class Client
{
private:
    int sock;

public:
    Client(char const *ip, unsigned short port);

    void run() const;

    void sendMessage(const std::string &msg) const;

    void receiveLoop() const;

    void sendLoop() const;
};
#endif//WOCKYTALK_CLIENT_HPP