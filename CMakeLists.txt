cmake_minimum_required(VERSION 3.6)
project(DataAnalysis)


file(GLOB_RECURSE source_file src/main/*.cpp)
set (CMAKE_BINARY_DIR )
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

find_package(Boost COMPONENTS serialization  REQUIRED)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS}) 
    add_executable(DataAnalysis ${source_file}) 
    target_link_libraries(DataAnalysis Boost::serialization)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -no-pie")

#g++ -o main -no-pie -I /opt/vcpkg/packages/matplotplusplus_x64-linux/include ../src/main/main.cpp -lboost_serialization


