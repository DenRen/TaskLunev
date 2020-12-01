cmake_minimum_required(VERSION 3.14)
project(Proxy)

set(CMAKE_CXX_STANDARD 17)

add_executable(Proxy main.cpp AddFunc.cpp DebugFunc.cpp LocalProxy.cpp LocalProxy.h)