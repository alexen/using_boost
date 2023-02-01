project(filesystem)
add_executable(${PROJECT_NAME}
     main.cpp
     )
target_link_libraries(${PROJECT_NAME}
     ${Boost_FILESYSTEM_LIBRARY}
     )
install(TARGETS ${PROJECT_NAME})