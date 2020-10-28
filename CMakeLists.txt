cmake_minimum_required(VERSION 3.17)
project(mapreduce)

set(CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -O -pthread")
add_executable(mapreduce mapreduce.cc wc.cc test_mapreduce.cc)