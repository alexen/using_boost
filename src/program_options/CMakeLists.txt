set(COMPONENT_NAME "program_options")

add_executable(${COMPONENT_NAME}
     main.cpp
)

target_link_libraries(${COMPONENT_NAME}
     PRIVATE
          Boost::program_options
)