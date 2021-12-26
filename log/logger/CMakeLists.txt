project(logger)

add_library(${PROJECT_NAME} logger.cpp)

target_link_libraries(${PROJECT_NAME}
     ${Boost_LOG_LIBRARY}
     )

target_compile_definitions(${PROJECT_NAME}
     PRIVATE BOOST_LOG_USE_NATIVE_SYSLOG
     )