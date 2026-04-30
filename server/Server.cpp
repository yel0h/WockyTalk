#include "Server.hpp"
#include "../common/MessageType.hpp"
#include "../core/Logger.hpp"
#include "ClientHandler.hpp"
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
        if (!msg.body.empty())
        {
            LOG("Received (" << msg.body.size() << " bytes): " << msg.body.data());
        }

        if (static_cast<MessageType>(msg.header.type) == MessageType::HELLO)
        {
            std::string name(msg.body.begin(), msg.body.end());
            std::lock_guard<std::mutex> lock(clientsMutex);
            for (auto &client : clients)
            {
                if (client.socket == msg.header.senderSocket)
                {
                    if (name.empty())
                    {
                        client.name = "User_" + std::to_string(client.id);
                    }
                    else
                    {
                        client.name = name;
                    }

                    LOG("Client " << client.id << " is now known as " << client.name);
                    break;
                }
            }

            continue;
        }

        broadcast(msg.header.senderSocket, msg.body.data(), msg.body.size());
    }
}

void Server::broadcast(int senderSocket, const char *message, unsigned long size)
{
    std::vector<Client> snapshot;

    {
        std::lock_guard<std::mutex> lock(clientsMutex);
        snapshot = clients;
    }

    std::string senderName;
    for (const auto &c : clients)
    {
        if (c.socket == senderSocket)
        {
            senderName = c.name;
            break;
        }
    }

    std::string final = "[" + senderName + "]: " + std::string(message, size);
    for (const auto& client : snapshot)
    {
        if (client.socket != senderSocket && send(client.socket, final.data(), final.size(), 0) == -1)
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
        Client client{};
        client.socket = clientSocket;
        client.id = nextClientId++;

        {
            std::lock_guard<std::mutex> lock(clientsMutex);
            clients.push_back(client);
        }

        std::thread(&ClientHandler::handleClient, ClientHandler(clientSocket, *this)).detach();
    }
}