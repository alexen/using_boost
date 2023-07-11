set(COMPONENT_NAME "stacktrace")

add_subdirectory(funcs)

add_executable(${COMPONENT_NAME} main.cpp)

target_link_libraries(${COMPONENT_NAME}
     funcs
     ${CMAKE_DL_LIBS}
     ${Boost_STACKTRACE_BASIC_LIBRARY}
)

install(TARGETS ${COMPONENT_NAME})
