project(stacktrace)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME}
     ${Boost_STACKTRACE_BASIC_LIBRARY}
     dl
     )