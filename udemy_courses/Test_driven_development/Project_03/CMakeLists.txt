cmake_minimum_required(VERSION 3.13)
project(sample_02)
set(CMAKE_CXX_STANDARD 11)
find_package(GTest REQUIRED)
message("GTEST_INCLUDE_DIRS = ${GTEST_INCLUDE_DIRS}")

add_library(commonLibrary LibraryCode.cpp)

add_executable(myExecutable main.cpp)
target_link_libraries(myExecutable commonLibrary)

add_executable(testRunner testRunner.cpp)

target_link_libraries(testRunner commonLibrary ${GTEST_LIBRARIES} pthread)

