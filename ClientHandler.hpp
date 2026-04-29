#ifndef WOCKYTALK_CLIENTHANDLER_HPP
#define WOCKYTALK_CLIENTHANDLER_HPP
class ClientHandler
{
private:
    int clientSocket;

public:
    explicit ClientHandler(int clientSocket) : clientSocket(clientSocket) {}

    void handleClient();
};
#endif//WOCKYTALK_CLIENTHANDLER_HPP
