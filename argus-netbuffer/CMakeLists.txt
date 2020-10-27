cmake_minimum_required(VERSION 3.13)  # CMake version check
set(CMAKE_CXX_STANDARD 17)            # Enable c++17 standard


add_library(argus-libnetbuffer STATIC)
add_library(argus::libnetbuffer ALIAS argus-libnetbuffer)

if(WIN32)
	target_link_libraries(argus-libnetbuffer wsock32 ws2_32)
endif()

if("${IDF_TARGET}" STREQUAL "esp32")
	target_link_libraries(argus-libnetbuffer PRIVATE idf::lwip)
endif()

if(CMAKE_GENERATOR MATCHES "Visual Studio")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()
target_sources(argus-libnetbuffer PRIVATE 	include/argus-netbuffer/netbuffer.h
											src/netbuffer.cpp
											include/argus-netbuffer/BasicMessageProtocol/BasicMessageProtocol.h
											src/BasicMessageProtocol/BasicMessageProtocol.cpp
											include/argus-netbuffer/netutils.h
											src/netutils.cpp)
target_include_directories(argus-libnetbuffer PUBLIC include/)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Test)