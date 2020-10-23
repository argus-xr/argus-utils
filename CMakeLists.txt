cmake_minimum_required(VERSION 3.13)  # CMake version check
project(argus-netbuffer)              # Create project "argus-netbuffer"
find_package(GTest)

if(GTEST_FOUND)
enable_testing()
endif()

add_subdirectory(argus-netbuffer)
