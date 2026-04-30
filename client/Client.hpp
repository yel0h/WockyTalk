#ifndef WOCKYTALK_CLIENT_HPP
#define WOCKYTALK_CLIENT_HPP
#include <string>
#ifdef _WIN32
#include <winsock2.h>

using Socket = SOCKET;
#else
using Socket = int;
#endif

class Client
{
private:
    int sock;

public:
    Client(char const *ip, unsigned short port, const std::string &nickname);

    void run() const;

    void sendMessage(const std::string &msg) const;

    void receiveLoop() const;

    void sendLoop() const;

    void sendHello(const std::string &nickname) const;
};
#endif//WOCKYTALK_CLIENT_HPP