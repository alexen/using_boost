set(EXECNAME iostreams)
add_executable(iostreams main.cpp)
target_link_libraries(iostreams
     ${Boost_IOSTREAMS_LIBRARY}
)

set(TESTNAME ${EXECNAME}-test)
add_executable(${TESTNAME}
     test/iostream_filters_test.cpp
)
target_link_libraries(${TESTNAME}
     ${Boost_IOSTREAMS_LIBRARY}
     ${OPENSSL_LIBRARIES}
     gtest
     gtest_main
)
add_test(${EXECNAME} ${TESTNAME})