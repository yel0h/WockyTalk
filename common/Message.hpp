#ifndef WOCKYTALK_MESSAGE_HPP
#define WOCKYTALK_MESSAGE_HPP
#include <vector>

struct Message
{
    int senderSocket;
    std::vector<char> data;
};
#endif//WOCKYTALK_MESSAGE_HPP