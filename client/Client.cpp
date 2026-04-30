#include "Client.hpp"
#include "../common/MessageHeader.hpp"
#include "../common/MessageType.hpp"
#include "../core/Logger.hpp"
#include <cstdlib>
#include <iostream>
#include <thread>
#ifdef _WIN32
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

Client::Client(char const *ip, unsigned short port, const std::string &nickname)
{
    std::cout << "Connecting to " << ip << ":" << port << std::endl;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        ERR("Socket creation failed");
        exit(1);
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &serverAddr.sin_addr);
    if (connect(sock, reinterpret_cast<sockaddr const *>(&serverAddr), sizeof(serverAddr)) == -1)
    {
        ERR("Connect failed");
#ifdef _WIN32
        closesocket(sock);
#else
        close(sock);
#endif
        exit(1);
    }

    sendHello(nickname);
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
    header.type = htonl(static_cast<unsigned int>(MessageType::CHAT));
    if (send(sock, reinterpret_cast<char const *>(&header), sizeof(header), 0) == -1 || send(sock, msg.data(), msg.size(), 0) == -1)
    {
        ERR("Send failed");
    }
}

void Client::receiveLoop() const
{
    char buffer[1024];
    while (true)
    {
        long bytes = recv(sock, buffer, sizeof(buffer), 0);
        if (bytes <= 0)
        {
            std::cout << "Server disconnected" << std::endl;
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
            shutdown(sock,
#ifdef _WIN32
                     SD_BOTH
#else
                     SHUT_RDWR
#endif
                     );
            break;
        }

        sendMessage(input);
    }
}

void Client::sendHello(const std::string &nickname) const
{
    MessageHeader header{};
    header.size = htonl(nickname.size());
    header.type = htonl(static_cast<unsigned int>(MessageType::HELLO));
    if (send(sock, reinterpret_cast<char const *>(&header), sizeof(header), 0) == -1 || send(sock, nickname.data(), nickname.size(), 0) == -1)
    {
        ERR("Send failed");
    }
}