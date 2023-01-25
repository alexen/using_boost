project(first)
add_executable(${PROJECT_NAME} first.cpp)
target_link_libraries(${PROJECT_NAME}
     ${OPENSSL_LIBRARIES}
     ${CMAKE_THREAD_LIBS_INIT}
     )
install(TARGETS ${PROJECT_NAME})