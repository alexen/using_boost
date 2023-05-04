# Для формирования sfx-архива используем утилиту [makeself](https://makeself.io/)
#
find_program(MAKESELF_PROGRAM makeself)
if(NOT MAKESELF_PROGRAM)
     message(FATAL_ERROR "'makeself' utility not found, please install it")
endif()

if(NOT SFX_INSTALLER_NAME)
     set(SFX_INSTALLER_NAME "${PROJECT_NAME}-${PROJECT_VERSION}-${CMAKE_SYSTEM_NAME}.sh")
endif()

if(NOT SFX_INSTALLER_DESCRIPTION)
     set(SFX_INSTALLER_DESCRIPTION "${PROJECT_NAME}: version ${PROJECT_VERSION} (${CMAKE_SYSTEM_NAME})")
endif()

if(SFX_INSTALLER_LICENSE_FILE)
     set(MAKESELF_PARAM_ADD_LICENSE "--license ${SFX_INSTALLER_LICENSE_FILE}")
endif()

set(TEMP_INSTALL_DIR ${CMAKE_BINARY_DIR}/_sfxtmp)
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
               ${MAKESELF_PARAM_ADD_LICENSE}
               ${TEMP_INSTALL_DIR}
               ${SFX_INSTALLER_NAME}
               ${SFX_INSTALLER_DESCRIPTION}
     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
     VERBATIM
)
add_custom_target(sfx DEPENDS ${SFX_INSTALLER_NAME})
