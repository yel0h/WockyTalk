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
        send(clientSocket, buffer, bytesReceived, 0);
    }

    close(clientSocket);
}