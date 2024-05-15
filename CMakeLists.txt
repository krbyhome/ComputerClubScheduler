cmake_minimum_required(VERSION 3.23)
project(YadroTask)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES main.cpp InputParser.cpp InputParser.hpp TableScheduler.cpp TableScheduler.hpp Event.hpp Logger.hpp Logger.cpp InvalidFormatException.hpp)
add_executable(YadroTask ${SOURCE_FILES})

enable_testing()
add_subdirectory(tests)