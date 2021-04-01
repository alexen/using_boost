cmake_minimum_required(VERSION 3.15)

project(using_boost)

add_definitions(-std=c++17)
add_definitions(-Wall -Werror -Wpedantic -Wextra)

set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -O0 -fno-inline")
set(BUILD_SHARED_LIBS ON)

include_directories(${CMAKE_SOURCE_DIR})

set(LOCAL_ROOT $ENV{HOME}/.local)
set(BOOST_ROOT ${LOCAL_ROOT})
find_package(Boost 1.69 REQUIRED
     serialization
     iostreams
     date_time
     coroutine
     thread
     locale
     )
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

add_subdirectory(googletest)
include_directories(${CMAKE_SOURCE_DIR}/googletest/googletest/include)
enable_testing()

add_subdirectory(core)
add_subdirectory(serialization)
add_subdirectory(iostreams)
add_subdirectory(coroutine2)
add_subdirectory(property_tree)
add_subdirectory(beast)
add_subdirectory(locale)
add_subdirectory(testing)
add_subdirectory(datetime)
add_subdirectory(regex)
add_subdirectory(variant)