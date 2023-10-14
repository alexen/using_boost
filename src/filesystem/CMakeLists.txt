set(COMPONENT_NAME "filesystem")

add_executable(${COMPONENT_NAME} main.cpp)

target_link_libraries(${COMPONENT_NAME}
     Boost::filesystem
)

install(TARGETS ${COMPONENT_NAME})
