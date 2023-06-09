cmake_minimum_required(VERSION 3.23)
project(csp_latinsquare)

set(CMAKE_CXX_STANDARD 14)

add_executable(csp_latinsquare main.cpp LatinSquare.cpp LatinSquare.h FileProcessing.h FileProcessing.cpp)
