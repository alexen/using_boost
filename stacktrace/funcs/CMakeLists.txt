project(funcs)
add_library(${PROJECT_NAME} funcs.cpp)
target_link_libraries(${PROJECT_NAME}
     ${Boost_STACKTRACE_BASIC_LIBRARY}
)
install(TARGETS ${PROJECT_NAME})