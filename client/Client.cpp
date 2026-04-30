#include "Client.hpp"
#include "../common/MessageHeader.hpp"
#include <arpa/inet.h>
#include <cstdlib>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <thread>

Client::Client(char const *ip, unsigned short port)
{
    sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &serverAddr.sin_addr);
    if (connect(sock, reinterpret_cast<sockaddr const *>(&serverAddr), sizeof(serverAddr)) == -1)
    {
        std::cerr << "Connect failed" << std::endl;
        exit(1);
    }
}

void Client::run() const
{
    std::thread receiver(&Client::receiveLoop, this);
    std::thread sender(&Client::sendLoop, this);
    sender.join();
    receiver.join();
}

void Client::sendMessage(const std::string &msg) const
{
    MessageHeader header{};
    header.size = htonl(msg.size());
    send(sock, &header, sizeof(header), 0);
    send(sock, msg.data(), msg.size(), 0);
}

void Client::receiveLoop() const
{
    char buffer[1024];
    while (true)
    {
        long bytes = recv(sock, buffer, sizeof(buffer), 0);
        if (bytes <= 0)
        {
            break;
        }

        std::cout << std::endl << std::string(buffer, bytes) << std::endl << "> " << std::flush;
    }
}

void Client::sendLoop() const
{
    std::string input;
    while (true)
    {
        std::cout << "> ";
        std::getline(std::cin, input);
        if (input.empty())
        {
            shutdown(sock, SHUT_RDWR);
            break;
        }

        sendMessage(input);
    }
}