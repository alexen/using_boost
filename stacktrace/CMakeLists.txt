project(stacktrace)

add_subdirectory(tools)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME}
     funcs
     ${CMAKE_DL_LIBS}
     ${Boost_STACKTRACE_BASIC_LIBRARY}
     )