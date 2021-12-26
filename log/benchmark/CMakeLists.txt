set(BM_NAME ${PROJECT_NAME}-benchmark)
add_executable(${BM_NAME}
     logger_benchmark.cpp
     main_benchmark.cpp
     )
target_link_libraries(${BM_NAME} celero logger)
add_dependencies(${PROJECT_NAME} ${BM_NAME})