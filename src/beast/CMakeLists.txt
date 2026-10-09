set(COMPONENT_NAME "beast")

find_package(Threads REQUIRED)

add_executable(${COMPONENT_NAME} first.cpp)

target_link_libraries(${COMPONENT_NAME}
     OpenSSL::SSL
     OpenSSL::Crypto
     Threads::Threads
)

install(TARGETS ${COMPONENT_NAME})
