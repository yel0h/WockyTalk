#include "Server.hpp"
#include "ClientHandler.hpp"
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

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

    if (listen(serverFd, 5) < 0)
    {
        std::cerr << "Listen failed" << std::endl;
        return;
    }

    std::cout << "Server listening on port " << port << "..." << std::endl;
    while (true)
    {
        int clientSocket = accept(serverFd, nullptr, nullptr);
        if (clientSocket < 0)
        {
            std::cerr << "Accept failed" << std::endl;
            break;
        }

        std::cout << "Client connected!" << std::endl;

        {
            std::lock_guard<std::mutex> lock(clientsMutex);
            clients.push_back(clientSocket);
        }

        std::thread(&ClientHandler::handleClient, ClientHandler(clientSocket, *this)).detach();
    }

    close(serverFd);
}

void Server::broadcast(int senderSocket, const char *message, unsigned long size)
{
    std::vector<int> snapshot;

    {
        std::lock_guard<std::mutex> lock(clientsMutex);
        snapshot = clients;
    }

    for (int client : snapshot)
    {
        if (client != senderSocket)
        {
            send(client, message, size, 0);
        }
    }
}