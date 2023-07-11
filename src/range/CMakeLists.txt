set(COMPONENT_NAME "range")

add_executable(${COMPONENT_NAME} main.cpp)

target_link_libraries(${COMPONENT_NAME} ${Boost_RANGE_LIBRARY})

install(TARGETS ${COMPONENT_NAME})
