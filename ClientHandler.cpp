#include "ClientHandler.hpp"
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>

void ClientHandler::handleClient()
{
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
        server.broadcast(clientSocket, buffer, bytesReceived);
    }

    {
        std::lock_guard<std::mutex> lock(server.clientsMutex);
        server.clients.erase(std::remove(server.clients.begin(), server.clients.end(), clientSocket), server.clients.end());
    }

    close(clientSocket);
}