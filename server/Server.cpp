#include "Server.hpp"
#include "ClientHandler.hpp"
#include "../core/Logger.hpp"
#include <netinet/in.h>
#include <sys/socket.h>
#include <thread>

void Server::start()
{
    serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd == -1)
    {
        ERR("Socket creation failed");
        return;
    }

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    if (bind(serverFd, reinterpret_cast<sockaddr const *>(&address), sizeof(address)) < 0)
    {
        ERR("Bind failed");
        return;
    }

    if (listen(serverFd, 5) < 0)
    {
        ERR("Listen failed");
        return;
    }

    LOG("Server listening on port " << port << "...");
    std::thread(&Server::acceptLoop, this).detach();
    while (true)
    {
        Message msg = messageQueue.pop();
        LOG("Received (" << msg.data.size() << " bytes): " << msg.data.data());
        broadcast(msg.senderSocket, msg.data.data(), msg.data.size());
    }
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
        if (client != senderSocket && send(client, message, size, 0) == -1)
        {
            ERR("Send failed");
        }
    }
}

void Server::acceptLoop()
{
    while (true)
    {
        int clientSocket = accept(serverFd, nullptr, nullptr);
        if (clientSocket < 0)
        {
            ERR("Accept failed");
            break;
        }

        LOG("Client connected!");

        {
            std::lock_guard<std::mutex> lock(clientsMutex);
            clients.push_back(clientSocket);
        }

        std::thread(&ClientHandler::handleClient, ClientHandler(clientSocket, *this)).detach();
    }
}