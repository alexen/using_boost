project(coroutine2)
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME}
     ${Boost_COROUTINE_LIBRARY}
     ${Boost_CONTEXT_LIBRARY}
     )