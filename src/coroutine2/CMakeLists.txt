set(COMPONENT_NAME "coroutine2")

add_executable(${COMPONENT_NAME} main.cpp)

target_link_libraries(${COMPONENT_NAME}
     ${Boost_COROUTINE_LIBRARY}
     ${Boost_CONTEXT_LIBRARY}
)

install(TARGETS ${COMPONENT_NAME})
