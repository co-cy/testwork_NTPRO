cmake_minimum_required(VERSION 3.21)
project(testwork_NTPRO)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

add_executable(Test
        src/main.cpp src/Order.h)
