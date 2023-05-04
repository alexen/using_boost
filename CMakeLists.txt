cmake_minimum_required(VERSION 3.15)

project(using_boost VERSION 1.0.0)

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

include(MakeSfx)

set(SRC_TAR_NAME "${PROJECT_NAME}.tar.bz2")
add_custom_command(
     OUTPUT ${SRC_TAR_NAME}
     COMMAND tar ARGS --exclude='./.*' --exclude='./celero' -cjf ${CMAKE_BINARY_DIR}/${SRC_TAR_NAME} .
     WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src
)
add_custom_target(srctgz DEPENDS ${CMAKE_BINARY_DIR}/${SRC_TAR_NAME})

set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_CONTACT "enakaev.aleksey@yandex.ru")
set(CPACK_PACKAGE_VENDOR "Enakaev Aleksey")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE.txt")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")
set(CPACK_PACKAGING_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")
set(CPACK_PACKAGE_CHECKSUM SHA512)
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA
     ${PROJECT_SOURCE_DIR}/package/deb/scripts/preinst
     ${PROJECT_SOURCE_DIR}/LICENSE.txt
     )

include(CPack)
