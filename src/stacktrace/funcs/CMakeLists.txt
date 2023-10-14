set(LIBRARY_NAME "funcs")

add_library(${LIBRARY_NAME} funcs.cpp)

target_link_libraries(${LIBRARY_NAME}
     Boost::stacktrace_basic
)

install(TARGETS ${LIBRARY_NAME})
