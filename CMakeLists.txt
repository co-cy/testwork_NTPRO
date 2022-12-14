cmake_minimum_required(VERSION 3.21)
project(testwork_NTPRO)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

set(shared_files
        src/Request/Request.h
        src/Response/Response.h
        src/Common.h
        src/User.h)

add_executable(Client
        ${shared_files}
        src/Client/Client.cpp src/Client/Client.h src/Client/main.cpp)

add_executable(Server
        ${shared_files}
        src/Server/Server.cpp src/Server/Server.h src/Server/main.cpp src/UserBase.h)
