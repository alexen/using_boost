add_executable(serialization main.cpp)
target_link_libraries(serialization
     ${Boost_DATE_TIME_LIBRARY}
     ${Boost_SERIALIZATION_LIBRARY}
     ${Boost_IOSTREAMS_LIBRARY}
     )