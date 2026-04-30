#include "Client.hpp"

int main(int argc, char *argv[])
{
    std::string ip = "127.0.0.1";
    int port = 6767;
    std::string nickname;
    if (argc >= 2)
    {
        ip = argv[1];
    }

    if (argc >= 3)
    {
        port = std::stoi(argv[2]);
    }

    if (argc >= 4)
    {
        nickname = argv[3];
    }

#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
#endif
    Client client(ip.c_str(), port, nickname);
    client.run();
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}