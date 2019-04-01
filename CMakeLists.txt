cmake_minimum_required(VERSION 3.13)
project(bpt)

set(CMAKE_CXX_STANDARD 11)

add_executable(bpt main.cpp bplustree.h bplustree.cpp)
