# Для формирования sfx-архива используем утилиту [makeself](https://makeself.io/)
#
find_program(MAKESELF_PROGRAM makeself)
if(NOT MAKESELF_PROGRAM)
     message(FATAL_ERROR "'makeself' utility not found, please install it")
endif()

set(SFX_INSTALLER_PREFIX "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_SYSTEM_NAME}")
set(SFX_INSTALLER_NAME "${SFX_INSTALLER_PREFIX}.sh")

set(TEMP_INSTALL_DIR /tmp/_${CMAKE_INSTALL_PREFIX})
add_custom_command(
     OUTPUT ${SFX_INSTALLER_NAME}
     COMMAND ${CMAKE_COMMAND}
          ARGS --install ${CMAKE_BINARY_DIR}
               --prefix ${TEMP_INSTALL_DIR}
     COMMAND ${MAKESELF_PROGRAM}
          ARGS --sha256
               --notemp
               --keep-umask
               --needroot
               --target ${CMAKE_INSTALL_PREFIX}
               --license ${CPACK_RESOURCE_FILE_LICENSE}
               ${TEMP_INSTALL_DIR}
               ${SFX_INSTALLER_NAME}
               ${SFX_INSTALLER_PREFIX}
     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
add_custom_target(sfx DEPENDS ${SFX_INSTALLER_NAME})
