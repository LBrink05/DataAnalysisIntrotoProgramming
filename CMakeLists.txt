cmake_minimum_required(VERSION 3.6)
project(DataAnalysis)

#source and output
file(GLOB_RECURSE source_file src/main/main.cpp)
set (CMAKE_BINARY_DIR)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

#find packages
find_package(Boost COMPONENTS serialization REQUIRED)

#include directories
include_directories(${Boost_INCLUDE_DIR})

#messages
message(STATUS "Boost_LIBRARIES = ${Boost_LIBRARIES}")

#create executable
add_executable(DataAnalysis ${source_file}) 

#link libraries to executable
target_link_libraries(DataAnalysis Boost::serialization)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -no-pie")

#g++ -o main -no-pie -I  ../src/main/main.cpp -lboost_serialization
