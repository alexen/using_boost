project(funcs)
add_library(${PROJECT_NAME} funcs.cpp)
install(TARGETS ${PROJECT_NAME})