cmake_minimum_required(VERSION 3.5)
project(provi2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lpthread")

configure_file(config.txt config.txt)
set(SOURCE_FILES main.cpp main.h rules.cpp rules.h)
add_executable(provi2 ${SOURCE_FILES})