cmake_minimum_required(VERSION 3.15)

project(using_boost VERSION 1.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
     set(CMAKE_INSTALL_PREFIX /opt/${PROJECT_NAME} CACHE PATH "" FORCE)
endif()

# Добавляем путь со своими модулями CMake
#
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")

add_definitions(-Wall -Werror -Wpedantic -Wextra)

set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -O0 -fno-inline")
option(BUILD_SHARED_LIBS "Build shared libs" ON)
option(BUILD_BENCHMARKS "Disable benchmarks build" OFF)

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
     stacktrace_basic
     log
     log_setup
     )
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

include(CTest)
enable_testing()
message(STATUS "Build testing: ${BUILD_TESTING}")

message(STATUS "Build benchmarks: ${BUILD_BENCHMARKS}")

set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib)

add_subdirectory(3rdparty)

include_directories(${CMAKE_SOURCE_DIR}/src)
add_subdirectory(src)

include(MakeSrcTgz)
include(MakeSfx)
include(MakePackage)
include(MakeSfxDeb)