set(LIBNAME func)
add_library(${LIBNAME} func.cpp)

set(EXECNAME testing)
add_executable(${EXECNAME} main.cpp)
target_link_libraries(${EXECNAME} ${LIBNAME})

set(TESTNAME ${EXECNAME}-test)
add_executable(${TESTNAME}
     initialize.cpp
     func_test.cpp
     dataset_test.cpp
)
target_link_libraries(${TESTNAME} ${LIBNAME})

add_test(${EXECNAME} ${TESTNAME})