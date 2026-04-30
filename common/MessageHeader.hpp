#ifndef WOCKYTALK_MESSAGEHEADER_HPP
#define WOCKYTALK_MESSAGEHEADER_HPP
struct MessageHeader
{
    unsigned int size;
    unsigned int type;
    int senderSocket;
};
#endif//WOCKYTALK_MESSAGEHEADER_HPP