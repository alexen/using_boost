project(iostreams)

add_executable(${PROJECT_NAME} main.cpp base64.cpp)
target_link_libraries(${PROJECT_NAME}
     ${Boost_IOSTREAMS_LIBRARY}
     )

if(BUILD_TESTING)
     set(TEST_NAME ${PROJECT_NAME}-test)
     add_executable(${TEST_NAME}
          base64.cpp
          test/main_test.cpp
          test/iostream_filters_test.cpp
          test/base64_test.cpp
          )
     target_link_libraries(${TEST_NAME}
          ${Boost_IOSTREAMS_LIBRARY}
          ${OPENSSL_LIBRARIES}
          ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
          )
     add_test(${PROJECT_NAME} ${TEST_NAME})
endif()

if(BUILD_BENCHMARKS)
     add_subdirectory(benchmark)
endif()