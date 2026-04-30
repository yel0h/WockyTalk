#include "ClientHandler.hpp"
#include "../core/Logger.hpp"
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>

void ClientHandler::handleClient()
{
    while (true)
    {
        MessageHeader header{};
        if (!recvAll(&header, sizeof(header)))
        {
            LOG("Client disconnected");
            break;
        }

        header.size = ntohl(header.size);
        header.type = ntohl(header.type);
        header.senderSocket = clientSocket;
        if (header.size > 1024)
        {
            SERR("Message too large!");
            break;
        }

        std::vector<char> body(header.size);
        if (!recvAll(body.data(), header.size))
        {
            LOG("Client disconnected");
            break;
        }

        Message msg;
        msg.header = header;
        msg.body.assign(body.begin(), body.end());
        server.messageQueue.push(std::move(msg));
    }

    {
        std::lock_guard<std::mutex> lock(server.clientsMutex);
        server.clients.erase(std::remove_if(server.clients.begin(), server.clients.end(), [&](const Client &c) { return c.socket == clientSocket; }), server.clients.end());
    }

    close(clientSocket);
}

bool ClientHandler::recvAll(void *buffer, unsigned long length) const
{
    unsigned long total = 0;
    char *buf = static_cast<char *>(buffer);
    while (total < length)
    {
        long bytes = recv(clientSocket, buf + total, length - total, 0);
        if (bytes <= 0)
        {
            return false;
        }

        total += bytes;
    }

    return true;
}