project(iostreams)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME}
     ${Boost_IOSTREAMS_LIBRARY}
     )

if(BUILD_TESTING)
     set(TEST_NAME ${PROJECT_NAME}-test)
     add_definitions(-DBOOST_TEST_MAIN)
     add_executable(${TEST_NAME}
          test/iostream_filters_test.cpp
          )
     target_link_libraries(${TEST_NAME}
          ${Boost_IOSTREAMS_LIBRARY}
          ${OPENSSL_LIBRARIES}
          ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
          )
     add_test(${PROJECT_NAME} ${TEST_NAME})
endif()