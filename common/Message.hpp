#ifndef WOCKYTALK_MESSAGE_HPP
#define WOCKYTALK_MESSAGE_HPP
#include "MessageHeader.hpp"
#include <vector>

struct Message
{
    MessageHeader header;
    std::vector<char> body;
};
#endif//WOCKYTALK_MESSAGE_HPP