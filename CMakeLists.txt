cmake_minimum_required(VERSION 3.21)
project(testwork_NTPRO)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

add_executable(Test
        src/main.cpp src/Order.h)

add_executable(Client
        src/Common.h src/Client/Client.cpp src/Client/Client.h src/Client/main.cpp)

add_executable(Server
        src/Common.h src/Server/Server.cpp src/Server/Server.h src/Server/main.cpp)
