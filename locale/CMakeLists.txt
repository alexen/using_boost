set(LIBNAME ediconvert)
add_library(${LIBNAME}
     src/charset_utils.cpp
)
target_link_libraries(${LIBNAME}
     ${Boost_LOCALE_LIBRARY}
)

if(BUILD_TESTING)
     set(TESTNAME ${LIBNAME}-test)
     add_definitions(-DBOOST_TEST_MAIN)
     add_executable(${TESTNAME}
          test/charset_utils_test.cpp
     )
     target_link_libraries(${TESTNAME}
          ${LIBNAME}
          ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
     )
     add_test(${LIBNAME} ${TESTNAME})
endif()