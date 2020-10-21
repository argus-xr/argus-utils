cmake_minimum_required(VERSION 3.13)  # CMake version check
project(argus-netbuffer)              # Create project "argus-netbuffer"
set(CMAKE_CXX_STANDARD 17)            # Enable c++17 standard


add_library(argus-libnetbuffer STATIC)

if(WIN32)
	target_link_libraries(argus-libnetbuffer wsock32 ws2_32)
endif()

if("${IDF_TARGET}" STREQUAL "esp32")
	target_link_libraries(argus-libnetbuffer PRIVATE idf::lwip)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
target_sources(argus-libnetbuffer PRIVATE netbuffer.h netbuffer.cpp BasicMessageProtocol/BasicMessageProtocol.h BasicMessageProtocol/BasicMessageProtocol.cpp netutils.h netutils.cpp)
target_include_directories(argus-libnetbuffer PUBLIC ../)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Test)