cmake_minimum_required(VERSION 3.5)

project(using_boost)

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

message(STATUS "Local include directories: ${LOCAL_INCLUDE_DIRS}")
message(STATUS "Local library directories: ${LOCAL_LIBRARY_DIRS}")

set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -O0 -fno-inline")

add_subdirectory(serialization)