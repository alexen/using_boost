set(MODULE_NAME clark)

add_library(${MODULE_NAME} MODULE
     src/clark.cpp
)

target_link_libraries(${MODULE_NAME}
     ${Boost_LOG_LIBRARY}
)

set_target_properties(${MODULE_NAME}
     PROPERTIES
          LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)