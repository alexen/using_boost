set(COMPONENT_NAME "regex")

add_executable(${COMPONENT_NAME} main.cpp)

target_link_libraries(${COMPONENT_NAME} ${Boost_REGEX_LIBRARY})

install(TARGETS ${COMPONENT_NAME})
