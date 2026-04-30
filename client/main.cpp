#include "Client.hpp"

int main()
{
#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
#endif
    Client client("127.0.0.1", 6767);
    client.run();
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}