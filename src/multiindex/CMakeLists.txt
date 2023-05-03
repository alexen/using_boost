project(multiindex)
add_executable(${PROJECT_NAME}
     main.cpp
     document.cpp
     )
install(TARGETS ${PROJECT_NAME})