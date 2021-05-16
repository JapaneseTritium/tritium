cmake_minimum_required(VERSION 3.16)
project(chuan C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_subdirectory(init)

