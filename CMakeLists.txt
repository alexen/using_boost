cmake_minimum_required(VERSION 3.15)

project(using_boost)

add_definitions(-std=c++17)
add_definitions(-Wall -Werror -Wpedantic -Wextra)

set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -O0 -fno-inline")
set(BUILD_SHARED_LIBS ON)

include_directories(${CMAKE_SOURCE_DIR})

find_package(Boost 1.69 REQUIRED
     unit_test_framework
     serialization
     iostreams
     date_time
     coroutine
     thread
     locale
     regex
     filesystem
     )
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

include(CTest)
enable_testing()
message(STATUS "Build testing: ${BUILD_TESTING}")

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
add_subdirectory(range)
add_subdirectory(multiindex)
add_subdirectory(filesystem)