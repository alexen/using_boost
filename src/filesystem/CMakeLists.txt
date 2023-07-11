set(COMPONENT_NAME "filesystem")

add_executable(${COMPONENT_NAME} main.cpp)

target_link_libraries(${COMPONENT_NAME}
     ${Boost_FILESYSTEM_LIBRARY}
)

install(TARGETS ${COMPONENT_NAME})
