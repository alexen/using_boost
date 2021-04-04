project(locale)

add_library(${PROJECT_NAME}
     src/charset_utils.cpp
     )
target_link_libraries(${PROJECT_NAME}
     ${Boost_LOCALE_LIBRARY}
     )

if(BUILD_TESTING)
     set(TEST_NAME ${PROJECT_NAME}-test)
     add_definitions(-DBOOST_TEST_MAIN)
     add_executable(${TEST_NAME}
          test/charset_utils_test.cpp
          )
     target_link_libraries(${TEST_NAME}
          ${PROJECT_NAME}
          ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
          )
     add_test(${PROJECT_NAME} ${TEST_NAME})
endif()