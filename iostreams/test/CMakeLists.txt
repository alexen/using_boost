set(TEST_NAME ${PROJECT_NAME}-test)
add_executable(${TEST_NAME}
     ../base64.cpp
     main_test.cpp
     iostream_filters_test.cpp
     base64_test.cpp
     )
target_link_libraries(${TEST_NAME}
     ${Boost_IOSTREAMS_LIBRARY}
     ${OPENSSL_LIBRARIES}
     ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
     )
add_test(${PROJECT_NAME} ${TEST_NAME})
add_dependencies(${PROJECT_NAME} ${TEST_NAME})