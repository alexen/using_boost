project(regex)
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} ${Boost_REGEX_LIBRARY})
install(TARGETS ${PROJECT_NAME})