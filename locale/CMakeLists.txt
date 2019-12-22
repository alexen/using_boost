set(LIBNAME ediconvert)
add_library(${LIBNAME}
     src/charset_utils.cpp
)
target_link_libraries(${LIBNAME}
     ${Boost_LOCALE_LIBRARY}
)

set(TESTNAME ${LIBNAME}-test)
add_executable(${TESTNAME}
     test/charset_utils_test.cpp
)
target_link_libraries(${TESTNAME}
     ${LIBNAME}
     gtest
     gtest_main
)

add_test(${LIBNAME} ${TESTNAME})