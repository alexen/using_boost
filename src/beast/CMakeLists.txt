set(COMPONENT_NAME "beast")

add_executable(${COMPONENT_NAME} first.cpp)

target_link_libraries(${COMPONENT_NAME}
     ${OPENSSL_LIBRARIES}
     ${CMAKE_THREAD_LIBS_INIT}
)

install(TARGETS ${COMPONENT_NAME})
