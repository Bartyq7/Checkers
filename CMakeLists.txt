cmake_minimum_required(VERSION 3.24)
project(checkers)

set(CMAKE_CXX_STANDARD 17)

add_executable(checkers main.cpp CheckersController.cpp board.cpp AI.cpp)