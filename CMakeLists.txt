cmake_minimum_required(VERSION 3.5)

project(using_boost)

add_definitions(-std=c++14)
add_definitions(-Wall -Werror -Wpedantic)

set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -O0 -fno-inline")
set(BUILD_SHARED_LIBS ON)

set(LOCAL_ROOT $ENV{HOME}/.local)
set(CMAKE_PREFIX_PATH ${LOCAL_ROOT})

find_package(Boost 1.69 REQUIRED
     serialization
     iostreams
     date_time
     coroutine
     context
     )
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

message(STATUS "Local include directories: ${LOCAL_INCLUDE_DIRS}")
message(STATUS "Local library directories: ${LOCAL_LIBRARY_DIRS}")

add_subdirectory(googletest)
include_directories(${CMAKE_SOURCE_DIR}/googletest/googletest/include)
enable_testing()

add_subdirectory(core)
add_subdirectory(serialization)
add_subdirectory(iostreams)
add_subdirectory(coroutine2)
add_subdirectory(property_tree)
add_subdirectory(beast)