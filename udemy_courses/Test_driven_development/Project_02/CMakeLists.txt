cmake_minimum_required(VERSION 3.13)
project(sample_02)
set(CMAKE_CXX_STANDARD 11)
find_package(GTest REQUIRED)
message("GTEST_INCLUDE_DIRS = ${GTEST_INCLUDE_DIRS}")

add_executable(myExecutable main.cpp)
target_link_libraries(myExecutable ${GTEST_LIBRARIES} pthread)



