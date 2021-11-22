project(testing)

set(TEST_NAME ${PROJECT_NAME}-test)
add_executable(${TEST_NAME}
     initialize.cpp
     dataset_test.cpp
     template_test.cpp
     test_suites_test.cpp
     fixture_test.cpp
     general_assertions_test.cpp
     enabling_disabling_test.cpp
     throw_test.cpp
     timeout_test.cpp
     ostream_test.cpp
     custom_output_test.cpp
)
target_link_libraries(${TEST_NAME}
     ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
     )
