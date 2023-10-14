set(MODULE_NAME alice)

add_library(${MODULE_NAME}
     src/alice.cpp
)

target_link_libraries(${MODULE_NAME}
     PRIVATE
          Boost::log
)

set_target_properties(${MODULE_NAME}
     PROPERTIES
          LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)