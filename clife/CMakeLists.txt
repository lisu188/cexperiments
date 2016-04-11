cmake_minimum_required(VERSION 3.3)
project(clife)

include(${CMAKE_SOURCE_DIR}/cmake/cotire.cmake)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_SOURCE_DIR}/cmake")

set(SOURCE_FILES
        clife.cpp
        clife.h)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wno-attributes")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3")

find_package(Boost 1.58 COMPONENTS system REQUIRED)
find_package(SDL2 REQUIRED)

include_directories(../../vstd)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(clife ${SOURCE_FILES})
target_link_libraries(clife ${Boost_LIBRARIES})

cotire(clife)



