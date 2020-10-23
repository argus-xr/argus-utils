cmake_minimum_required(VERSION 3.13)  # CMake version check

if(GTEST_FOUND)
	set(BINARYFILE ${CMAKE_PROJECT_NAME}_test)
	add_executable(${BINARYFILE} main.cpp VarInt.cpp)
	#add_test(NAME ${BINARYFILE} COMMAND ${BINARYFILE})
	gtest_discover_tests(${BINARYFILE})
	target_link_libraries(${BINARYFILE} PUBLIC argus-libnetbuffer GTest::GTest GTest::Main)
endif()