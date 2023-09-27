set(COMPONENT_NAME "log")

add_executable(${COMPONENT_NAME}
     main.cpp
     handlers.cpp
     application.cpp
)
target_link_libraries(${COMPONENT_NAME}
     logger
     dynlib
     ${Boost_LOG_LIBRARY}
)

add_subdirectory(logger)

if(BUILD_BENCHMARKS)
     add_subdirectory(benchmark)
endif()

install(TARGETS ${COMPONENT_NAME})

add_subdirectory(modules)
