set(MODULE_NAME alice)

add_library(${MODULE_NAME}
     src/alice.cpp
)

target_link_libraries(${MODULE_NAME}
     PRIVATE
          /home/alexen/.skel/build/current/boost_1_72_0/stage/lib/libboost_chrono.a
          /home/alexen/.skel/build/current/boost_1_72_0/stage/lib/libboost_regex.a
          /home/alexen/.skel/build/current/boost_1_72_0/stage/lib/libboost_atomic.a
          /home/alexen/.skel/build/current/boost_1_72_0/stage/lib/libboost_log_setup.a
          /home/alexen/.skel/build/current/boost_1_72_0/stage/lib/libboost_filesystem.a
          /home/alexen/.skel/build/current/boost_1_72_0/stage/lib/libboost_date_time.a
          /home/alexen/.skel/build/current/boost_1_72_0/stage/lib/libboost_thread.a
          /home/alexen/.skel/build/current/boost_1_72_0/stage/lib/libboost_log.a
)
