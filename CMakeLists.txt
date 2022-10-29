cmake_minimum_required(VERSION 3.21)
project(http_proxy VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)

add_executable(http_proxy src/main.cpp src/Socket/Socket.cpp src/Socket/Socket.h src/Server/Server.cpp src/Server/Server.h)
