cmake_minimum_required(VERSION 3.5)

project(using_boost)

add_definitions(-std=c++14)
add_definitions(-Wall -Werror -Wpedantic)

set(LOCAL_ROOT $ENV{HOME}/.local)
set(LOCAL_INCLUDE_DIRS
     ${LOCAL_ROOT}/include
     ${CMAKE_SOURCE_DIR}
     )
set(LOCAL_LIBRARY_DIRS
     ${LOCAL_ROOT}/lib
     ${LOCAL_ROOT}/lib64
     )

include_directories(SYSTEM ${LOCAL_INCLUDE_DIRS})
link_directories(${LOCAL_LIBRARY_DIRS} ${LOCAL_LIBRARY64_DIR})

find_package(Boost 1.69 REQUIRED
     serialization
     iostreams
     date_time
     )
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

message(STATUS "Local include directories: ${LOCAL_INCLUDE_DIRS}")
message(STATUS "Local library directories: ${LOCAL_LIBRARY_DIRS}")

set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -O0 -fno-inline")

enable_testing()
add_subdirectory(googletest)
add_subdirectory(serialization)
add_subdirectory(iostreams)