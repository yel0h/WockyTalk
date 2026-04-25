#ifndef WOCKYTALK_SERVER_HPP
#define WOCKYTALK_SERVER_HPP
class Server
{
private:
    unsigned short port;
    int serverFd{};

public:
    explicit Server(unsigned short port) : port(port) {}

    void start();
};
#endif//WOCKYTALK_SERVER_HPP