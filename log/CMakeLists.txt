project(log)
add_executable(${PROJECT_NAME}
     main.cpp
     )
target_link_libraries(${PROJECT_NAME}
     ${Boost_LOG_LIBRARY}
     )