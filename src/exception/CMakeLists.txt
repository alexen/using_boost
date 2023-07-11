set(COMPONENT_NAME "exception")

set(LIBRARY_NAME "tools")
add_library(${LIBRARY_NAME} tools.cpp)

add_executable(${COMPONENT_NAME} main.cpp)

target_link_libraries(${COMPONENT_NAME} ${LIBRARY_NAME})

install(TARGETS ${COMPONENT_NAME})
