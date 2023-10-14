set(COMPONENT_NAME "locale")

add_library(${COMPONENT_NAME}
     src/charset_utils.cpp
)

# Use self-documented variables for boolean generator expressions
# @see https://cmake.org/cmake/help/latest/manual/cmake-generator-expressions.7.html#whitespace-and-quoting
#
set(IF_STATIC_BUILD "$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>")
#
# WARNING! In static linked order matters!
#
set(ICU_STATIC_LIBS
     libicui18n.a
     libicuuc.a
     libicudata.a
     libicuio.a
     libicutu.a
)

target_link_libraries(${COMPONENT_NAME}
     Boost::locale
     "$<${IF_STATIC_BUILD}:${ICU_STATIC_LIBS}>"
)

if(BUILD_TESTING)
     set(TEST_NAME ${COMPONENT_NAME}-test)
     add_definitions(-DBOOST_TEST_MAIN)
     add_executable(${TEST_NAME}
          test/charset_utils_test.cpp
     )
     target_link_libraries(${TEST_NAME}
          ${COMPONENT_NAME}
          Boost::unit_test_framework
     )
     add_test(${COMPONENT_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_NAME})
endif()

install(TARGETS ${COMPONENT_NAME})
