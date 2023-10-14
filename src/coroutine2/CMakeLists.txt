set(COMPONENT_NAME "coroutine2")

add_executable(${COMPONENT_NAME} main.cpp)

target_link_libraries(${COMPONENT_NAME}
     Boost::coroutine
     Boost::context
)

install(TARGETS ${COMPONENT_NAME})
