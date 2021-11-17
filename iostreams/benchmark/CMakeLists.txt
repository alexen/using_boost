set(SRC
     base64_benchmark.cpp
     iostream_benchmark.cpp
     )
foreach(EACH_SRC ${SRC})
     get_filename_component(TARGET ${EACH_SRC} NAME_WE)
     add_executable(${TARGET}
          ${EACH_SRC}
          ../base64.cpp
          )
     target_link_libraries(${TARGET} celero)
endforeach()
