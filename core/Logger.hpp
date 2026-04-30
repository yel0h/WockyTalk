#ifndef WOCKYTALK_LOGGER_HPP
#define WOCKYTALK_LOGGER_HPP
#include <cstring>
#include <iostream>
#define LOG(msg) std::cout << "[LOG] " << msg << std::endl
#define SERR(msg) std::cerr << "[ERR] " << msg << std::endl
#ifdef _WIN32
#define ERR(msg) SERR(msg)
#else
#define ERR(msg) std::cerr << "[ERR] " << msg << ": " << strerror(errno) << std::endl
#endif
#endif//WOCKYTALK_LOGGER_HPP