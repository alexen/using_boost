set(THIS thread)

add_executable(${THIS} main.cpp)
target_link_libraries(${THIS}
     PRIVATE
          Boost::thread
          Boost::log
)