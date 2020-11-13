cmake_minimum_required(VERSION 3.14)
project(FWRonSemShm)

set(CMAKE_CXX_STANDARD 17)

add_executable(FWRonSemShm main.cpp ../AddLib/AddFunc.h ../AddLib/AddFunc.cpp wrrd.cpp wrrd.h)