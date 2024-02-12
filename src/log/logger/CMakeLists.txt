set(LIBRARY_NAME "logger")

add_library(${LIBRARY_NAME}
     src/sinks.cpp
     src/initializer.cpp
)

target_link_libraries(${LIBRARY_NAME}
     Boost::log
     Boost::log_setup
)

target_compile_definitions(${LIBRARY_NAME}
     PRIVATE
          BOOST_LOG_USE_NATIVE_SYSLOG
)

install(TARGETS ${LIBRARY_NAME})
