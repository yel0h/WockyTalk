#include "Client.hpp"

int main()
{
    Client client("127.0.0.1", 6767);
    client.run();
    return 0;
}