set(COMPONENT_NAME "serialization")

add_executable(${COMPONENT_NAME} main.cpp)

# Use self-documented variables for boolean generator expressions
# @see https://cmake.org/cmake/help/latest/manual/cmake-generator-expressions.7.html#whitespace-and-quoting
#
set(IF_STATIC_BUILD "$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>")

target_link_libraries(${COMPONENT_NAME}
     Boost::date_time
     Boost::serialization
     Boost::iostreams
     "$<${IF_STATIC_BUILD}:libz.a>"
)

install(TARGETS ${COMPONENT_NAME})
