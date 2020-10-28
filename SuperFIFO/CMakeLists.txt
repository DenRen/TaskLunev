cmake_minimum_required(VERSION 3.14)
project(SuperFIFO)

set(CMAKE_CXX_STANDARD 17)

add_executable(SuperFIFO SmartFIFO.cpp SmartFIFO.h semaphore.cpp semaphore.cpp semaphore.h main.cpp wrrd.h wrrd.cpp)