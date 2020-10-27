add_executable(datetime main.cpp)
target_link_libraries(datetime
     ${Boost_DATE_TIME_LIBRARY}
     )