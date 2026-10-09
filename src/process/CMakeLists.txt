set(COMPONENT_NAME "process")
add_executable(${COMPONENT_NAME}
     main.cpp
)
target_link_libraries(${COMPONENT_NAME}
     PRIVATE
          Boost::filesystem
)