cmake_minimum_required(VERSION 3.23)
project(4Laba)

set(CMAKE_CXX_STANDARD 14)

add_executable(4Laba
        src/main.cpp
        include/SparseMatrix.h
        include/SparseVector.h)
