#ifndef WOCKYTALK_CLIENTHANDLER_HPP
#define WOCKYTALK_CLIENTHANDLER_HPP
#include "Server.hpp"

class ClientHandler
{
private:
    int clientSocket;
    Server &server;

public:
    explicit ClientHandler(int clientSocket, Server &server) : clientSocket(clientSocket), server(server) {}

    void handleClient();

    bool recvAll(void *buffer, unsigned long length) const;
};
#endif//WOCKYTALK_CLIENTHANDLER_HPP