cmake_minimum_required(VERSION 3.13)  # CMake version check
project(Google_test_sample)

add_library(Sum_library STATIC Libraries/sum.cpp)

#get google test

include(FetchContent)
FetchContent_Declare(googletest
	GIT_REPOSITORY https://github.com/google/googletest
	GIT_TAG release-1.11.0)
FetchContent_GetProperties(googletest)
#googletest_POPULATED
#googletest_SOURCE_DIR
#googletest_BUILD_DIR
if(NOT googletest_POPULATED)
	FetchContent_Populate(googletest)
	add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BUILD_DIR})
endif()
add_executable(Main main.cpp)
target_link_libraries(Main Sum_library)


add_executable(Test test.cpp)
target_link_libraries(Test Sum_library gtest_main gmock_main)

