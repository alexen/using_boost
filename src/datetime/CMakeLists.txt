set(COMPONENT_NAME "datetime")

add_executable(${COMPONENT_NAME} main.cpp)

target_link_libraries(${COMPONENT_NAME}
     ${Boost_DATE_TIME_LIBRARY}
)

install(TARGETS ${COMPONENT_NAME})
