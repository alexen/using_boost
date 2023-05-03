project(iostreams)

add_executable(${PROJECT_NAME} main.cpp base64.cpp)
target_link_libraries(${PROJECT_NAME}
     ${Boost_IOSTREAMS_LIBRARY}
     )

if(BUILD_TESTING)
     add_subdirectory(test)
endif()

if(BUILD_BENCHMARKS)
     add_subdirectory(benchmark)
endif()

install(TARGETS ${PROJECT_NAME})