project(serialization)
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME}
     ${Boost_DATE_TIME_LIBRARY}
     ${Boost_SERIALIZATION_LIBRARY}
     ${Boost_IOSTREAMS_LIBRARY}
     )
install(TARGETS ${PROJECT_NAME})