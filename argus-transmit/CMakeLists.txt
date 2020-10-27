cmake_minimum_required(VERSION 3.13)  # CMake version check
set(CMAKE_CXX_STANDARD 17)            # Enable c++17 standard


add_library(argus-transmit STATIC)
add_library(argus::transmit ALIAS argus-transmit)

if(NOT TARGET argus::libnetbuffer)
	find_package(argus-libnetbuffer REQUIRED)
endif()

target_link_libraries(argus-transmit PRIVATE argus::libnetbuffer)

if(WIN32)
	target_link_libraries(argus-transmit PRIVATE wsock32 ws2_32)
endif()

if("${IDF_TARGET}" STREQUAL "esp32")
	target_link_libraries(argus-transmit PRIVATE idf::lwip)
endif()

if(CMAKE_GENERATOR MATCHES "Visual Studio")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()
target_sources(argus-transmit PRIVATE 	include/argus-transmit/ConnectionBase.h
										include/argus-transmit/ConnectionHandler.h
										include/argus-transmit/QueueBase.h
										include/argus-transmit/QueueSingle.h
										src/QueueSingle.cpp
										include/argus-transmit/QueueMany.h
										src/QueueMany.cpp
										src/ConnectionHandler.cpp)
target_include_directories(argus-transmit PUBLIC include)