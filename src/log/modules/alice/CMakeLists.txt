set(MODULE_NAME alice)

add_library(${MODULE_NAME} MODULE
     src/alice.cpp
)

set_target_properties(${MODULE_NAME}
     PROPERTIES
          LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)