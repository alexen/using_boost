set(SFXDEB_INSTALL_FILE "${PROJECT_NAME}-${PROJECT_VERSION}-${CMAKE_SYSTEM_NAME}.deb.sh")

add_custom_command(
     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
     OUTPUT ${SFXDEB_INSTALL_FILE}
     COMMAND tr -d "\r" < ${CMAKE_SOURCE_DIR}/package/sfxdeb/install.sh.in > ${SFXDEB_INSTALL_FILE}
     COMMAND make package
     COMMAND ls -1t "*.deb" | head -1 | xargs cat >> ${SFXDEB_INSTALL_FILE}
     COMMAND chmod a+x ${SFXDEB_INSTALL_FILE}
)

add_custom_target(sfxdeb
     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
     DEPENDS ${SFXDEB_INSTALL_FILE}
)