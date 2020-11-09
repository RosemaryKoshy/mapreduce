cmake_minimum_required(VERSION 3.17)
project(mapreduce)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -ggdb3 -Og -pthread")
add_executable(wc mapreduce.cc wc.cc)
add_executable(grep mapreduce.cc grep.cc)