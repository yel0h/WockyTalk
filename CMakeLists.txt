cmake_minimum_required(VERSION 3.30)
project(WockyTalk)

set(CMAKE_CXX_STANDARD 26)

add_executable(WockyTalkServer server/main.cpp
        server/Server.cpp
        server/Server.hpp
        server/ClientHandler.cpp
        server/ClientHandler.hpp
        common/MessageHeader.hpp
        common/Message.hpp
        core/MessageQueue.cpp
        core/MessageQueue.hpp
        core/Logger.hpp)

add_executable(WockyTalkClient client/main.cpp
        common/MessageHeader.hpp
        client/Client.cpp
        client/Client.hpp
        core/Logger.hpp)