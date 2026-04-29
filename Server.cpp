#include "Server.hpp"
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

void Server::start()
{
    serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd == -1)
    {
        std::cerr << "Socket creation failed" << std::endl;
        return;
    }

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    if (bind(serverFd, reinterpret_cast<sockaddr const *>(&address), sizeof(address)) < 0)
    {
        std::cerr << "Bind failed" << std::endl;
        return;
    }

    if (listen(serverFd, 1) < 0)
    {
        std::cerr << "Listen failed" << std::endl;
        return;
    }

    std::cout << "Server listening on port " << port << "..." << std::endl;
    sockaddr clientAddr{};
    unsigned int clientSize = sizeof(clientAddr);
    int clientSocket = accept(serverFd, &clientAddr, &clientSize);
    if (clientSocket < 0)
    {
        std::cerr << "Accept failed" << std::endl;
        return;
    }

    std::cout << "Client connected!" << std::endl;
    char buffer[1024];
    while (true)
    {
        long bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived <= 0)
        {
            std::cout << "Client disconnected" << std::endl;
            break;
        }

        buffer[bytesReceived] = '\0';
        std::cout << "Received (" << bytesReceived << " bytes): " << buffer;
        send(clientSocket, buffer, bytesReceived, 0);
    }

    close(clientSocket);
    close(serverFd);
}