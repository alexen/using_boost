add_executable(first first.cpp)
target_link_libraries(first ${OPENSSL_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})