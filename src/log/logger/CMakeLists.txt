set(LIBRARY_NAME "logger")

add_library(${LIBRARY_NAME} logger.cpp)

target_link_libraries(${LIBRARY_NAME}
     ${Boost_LOG_LIBRARY}
)

target_compile_definitions(${LIBRARY_NAME}
     PRIVATE BOOST_LOG_USE_NATIVE_SYSLOG
)

install(TARGETS ${LIBRARY_NAME})
