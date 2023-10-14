set(COMPONENT_NAME "core")

if(BUILD_TESTING)
     set(TEST_NAME ${COMPONENT_NAME}-test)
     add_definitions(-DBOOST_TEST_MAIN)
     add_executable(${TEST_NAME}
          test/core_test.cpp
     )
     target_link_libraries(${TEST_NAME}
          Boost::unit_test_framework
     )
     add_test(${COMPONENT_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_NAME})
endif()