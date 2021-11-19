set(BM_NAME ${PROJECT_NAME}-benchmark)
add_executable(${BM_NAME}
     ../base64.cpp
     base64_benchmark.cpp
     iostream_benchmark.cpp
     filters_benchmark.cpp
     main_benchmark.cpp
     )
target_link_libraries(${BM_NAME} celero)
add_dependencies(${PROJECT_NAME} ${BM_NAME})