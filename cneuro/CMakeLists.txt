cmake_minimum_required(VERSION 3.3)
project(cneuro)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3")

set(SOURCE_FILES cneuro.cpp)

find_package(Boost 1.58 COMPONENTS system REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/../vstd)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(cneuro ${SOURCE_FILES})

target_link_libraries(cneuro ${Boost_LIBRARIES})