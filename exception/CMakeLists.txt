project(exception)

set(INNER_LIB_NAME tools)

add_library(${INNER_LIB_NAME} tools.cpp)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} ${INNER_LIB_NAME})
