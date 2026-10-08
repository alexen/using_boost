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

include(GNUInstallDirs)

set(CMAKE_INSTALL_RPATH $ORIGIN/../${CMAKE_INSTALL_LIBDIR})
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

set(RESULT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/stage)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${RESULT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${RESULT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${RESULT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})

set(CMAKE_CXX_FLAGS_ALL_WARNINGS "-Wall -Wpedantic -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -O0 -fno-inline")
option(BUILD_SHARED_LIBS "Build shared libs" ON)
option(BUILD_BENCHMARKS "Disable benchmarks build" OFF)

# ---------------------------------------------------------------------------
# Boost из исходников (FetchContent)
# ---------------------------------------------------------------------------
include(FetchContent)
FetchContent_Declare(Boost
     URL "https://github.com/boostorg/boost/releases/download/boost-1.85.0/boost-1.85.0-cmake.tar.gz"
     DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(BOOST_INCLUDE_LIBRARIES
     test
     serialization
     iostreams
     date_time
     coroutine
     thread
     locale
     regex
     filesystem
     stacktrace
     log
     program_options
     random
)
set(BOOST_ENABLE_CMAKE ON)
FetchContent_MakeAvailable(Boost)

find_package(Doxygen QUIET)
if(DOXYGEN_FOUND)
     doxygen_add_docs(doxygen)
endif()

find_package(OpenSSL REQUIRED)

include(CTest)
message(STATUS "Build testing: ${BUILD_TESTING}")
message(STATUS "Build benchmarks: ${BUILD_BENCHMARKS}")

add_subdirectory(3rdparty)

include_directories(${CMAKE_SOURCE_DIR}/src)
add_subdirectory(src)

include(MakeSrcTgz)
include(MakeSfx)
include(MakePackage)
include(MakeSfxDeb)
