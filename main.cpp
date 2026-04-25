#include "Server.hpp"

int main() {
    Server server(6767);
    server.start();
    return 0;
}
