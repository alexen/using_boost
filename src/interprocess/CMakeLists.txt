set(COMPONENT_NAME "interprocess")
add_executable(${COMPONENT_NAME}
     main.cpp
)
target_link_libraries(${COMPONENT_NAME} PRIVATE ${Boost_THREAD_LIBRARY})