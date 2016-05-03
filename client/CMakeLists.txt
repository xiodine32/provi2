cmake_minimum_required(VERSION 3.5)
project(provi2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lpthread -lSDL -lSDL_image -lGL -lGLU")

configure_file(font_2.bmp font_2.bmp COPYONLY)
set(SOURCE_FILES main.cpp)
add_executable(provi2 ${SOURCE_FILES})