#include "ClientHandler.hpp"
#include "../common/MessageHeader.hpp"
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

        unsigned int size = ntohl(header.size);
        if (size > 1024)
        {
            SERR("Message too large!");
            break;
        }

        std::vector<char> body(size);
        if (!recvAll(body.data(), size))
        {
            LOG("Client disconnected");
            break;
        }

        Message msg;
        msg.senderSocket = clientSocket;
        msg.data.assign(body.begin(), body.end());
        server.messageQueue.push(std::move(msg));
    }

    {
        std::lock_guard<std::mutex> lock(server.clientsMutex);
        server.clients.erase(std::remove(server.clients.begin(), server.clients.end(), clientSocket), server.clients.end());
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