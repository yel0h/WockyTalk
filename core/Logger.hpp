#ifndef WOCKYTALK_LOGGER_HPP
#define WOCKYTALK_LOGGER_HPP
#include <cstring>
#include <iostream>
#define LOG(msg) std::cout << "[LOG] " << msg << std::endl
#define ERR(msg) std::cerr << "[ERR] " << msg << ": " << strerror(errno) << std::endl
#define SERR(msg) std::cerr << "[ERR] " << msg << std::endl
#endif//WOCKYTALK_LOGGER_HPP