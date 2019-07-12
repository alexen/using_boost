add_executable(coroutine2 main.cpp)
target_link_libraries(coroutine2 ${Boost_COROUTINE_LIBRARY} ${Boost_CONTEXT_LIBRARY})