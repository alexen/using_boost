set(SRC_TAR_NAME "${PROJECT_NAME}.tar.gz")
add_custom_command(
     OUTPUT ${SRC_TAR_NAME}
     COMMAND tar ARGS --exclude='./.*' --exclude='./celero' -czf ${CMAKE_BINARY_DIR}/${SRC_TAR_NAME} ./src
     WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
add_custom_target(srctgz DEPENDS ${CMAKE_BINARY_DIR}/${SRC_TAR_NAME})
