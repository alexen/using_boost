set(COMPONENT_NAME "iostreams")

add_executable(${COMPONENT_NAME}
     main.cpp
     base64.cpp
)

target_link_libraries(${COMPONENT_NAME}
     ${Boost_IOSTREAMS_LIBRARY}
)

if(BUILD_TESTING)
     set(TEST_NAME ${COMPONENT_NAME}-test)
     add_executable(${TEST_NAME}
          base64.cpp
          test/main_test.cpp
          test/base64_test.cpp
          test/filters_test.cpp
     )
     target_link_libraries(${TEST_NAME}
          ${Boost_IOSTREAMS_LIBRARY}
          ${OPENSSL_LIBRARIES}
          ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
     )
     add_test(${COMPONENT_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_NAME})
     add_dependencies(${COMPONENT_NAME} ${TEST_NAME})
endif()

if(BUILD_BENCHMARKS)
     set(BENCHMARK_NAME ${COMPONENT_NAME}-benchmark)
     add_executable(${BENCHMARK_NAME}
          base64.cpp
          benchmark/base64_benchmark.cpp
          benchmark/iostream_benchmark.cpp
          benchmark/filters_benchmark.cpp
          benchmark/main_benchmark.cpp
     )
     target_link_libraries(${BENCHMARK_NAME} celero)
     add_dependencies(${COMPONENT_NAME} ${BENCHMARK_NAME})
endif()

install(TARGETS ${COMPONENT_NAME})
