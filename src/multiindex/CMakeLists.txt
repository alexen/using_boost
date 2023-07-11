set(COMPONENT_NAME "multiindex")

add_executable(${COMPONENT_NAME}
     main.cpp
     document.cpp
)

install(TARGETS ${COMPONENT_NAME})
