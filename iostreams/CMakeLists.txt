add_executable(iostreams main.cpp)
target_link_libraries(iostreams
     ${Boost_IOSTREAMS_LIBRARY}
     )