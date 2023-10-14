set(COMPONENT_NAME "testing")

add_executable(${COMPONENT_NAME}
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

target_link_libraries(${COMPONENT_NAME}
     PRIVATE
          Boost::unit_test_framework
)

install(TARGETS ${COMPONENT_NAME})
