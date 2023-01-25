project(log)
add_executable(${PROJECT_NAME}
     main.cpp
     handlers.cpp
     application.cpp
     )
target_link_libraries(${PROJECT_NAME}
     logger
     ${Boost_LOG_LIBRARY}
     )
add_subdirectory(logger)

if(BUILD_BENCHMARKS)
     add_subdirectory(benchmark)
endif()

install(TARGETS ${PROJECT_NAME})