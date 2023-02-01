cmake_minimum_required(VERSION 3.15)

project(using_boost)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-Wall -Werror -Wpedantic -Wextra)

set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -O0 -fno-inline")
option(BUILD_SHARED_LIBS "Build shared libs" ON)
option(BUILD_BENCHMARKS "Enable benchmarks build" ON)

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
if(BUILD_BENCHMARKS)
     add_subdirectory(celero)
     include_directories(celero/include)
endif()

set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib)

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
add_subdirectory(tribool)
add_subdirectory(stacktrace)
add_subdirectory(log)
add_subdirectory(algorithm)
add_subdirectory(exception)

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
set(CPACK_DEBIAN_PACKAGE_VERSION "1.1.0")
set(CPACK_PACKAGE_CONTACT "enakaev.aleksey@yandex.ru")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Enakaev Aleksey")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE.txt")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")
set(CPACK_PACKAGING_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")
set(CPACK_PACKAGE_CHECKSUM SHA512)
set(CPACK_DEBIAN_PACKAGE_FILE_NAME "${PROJECT_NAME}-${CPACK_DEBIAN_PACKAGE_VERSION}-${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_DEBIAN_PACKAGE_FILE_NAME}")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA
     ${PROJECT_SOURCE_DIR}/deb/scripts/preinst
     ${PROJECT_SOURCE_DIR}/LICENSE.txt
     )

include(CPack)
