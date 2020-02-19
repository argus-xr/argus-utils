cmake_minimum_required(VERSION 3.13)  # CMake version check
project(argus-netbuffer)              # Create project "argus-netbuffer"
set(CMAKE_CXX_STANDARD 17)            # Enable c++17 standard

add_library(argus-libnetbuffer STATIC)
target_sources(argus-libnetbuffer PRIVATE netbuffer.h netbuffer.cpp)