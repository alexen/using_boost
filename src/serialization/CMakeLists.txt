project(serialization)
add_executable(${PROJECT_NAME} main.cpp)

# Use self-documented variables for boolean generator expressions
# @see https://cmake.org/cmake/help/latest/manual/cmake-generator-expressions.7.html#whitespace-and-quoting
#
set(IF_STATIC_BUILD "$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>")

target_link_libraries(${PROJECT_NAME}
     ${Boost_DATE_TIME_LIBRARY}
     ${Boost_SERIALIZATION_LIBRARY}
     ${Boost_IOSTREAMS_LIBRARY}
     "$<${IF_STATIC_BUILD}:libz.a>"
)
install(TARGETS ${PROJECT_NAME})