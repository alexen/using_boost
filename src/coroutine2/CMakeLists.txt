set(COMPONENT_NAME "coroutine2")

add_executable(${COMPONENT_NAME} main.cpp)

target_link_libraries(${COMPONENT_NAME}
     Boost::coroutine
     Boost::context
)

# Boost 1.83+ генерирует false-positive -Wmaybe-uninitialized в coroutine2/detail/state.hpp
target_compile_options(${COMPONENT_NAME} PRIVATE -Wno-maybe-uninitialized)

install(TARGETS ${COMPONENT_NAME})
