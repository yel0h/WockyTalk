cmake_minimum_required(VERSION 3.30)
project(WockyTalk)

set(CMAKE_CXX_STANDARD 26)

add_executable(WockyTalk main.cpp
        Server.cpp
        Server.hpp)
