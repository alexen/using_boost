set(COMPONENT_NAME "stacktrace")

add_subdirectory(funcs)

add_executable(${COMPONENT_NAME} main.cpp)

target_link_libraries(${COMPONENT_NAME}
     funcs
     Boost::stacktrace_basic
     ${CMAKE_DL_LIBS}
)

install(TARGETS ${COMPONENT_NAME})
