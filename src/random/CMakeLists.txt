set(THIS "random")

add_executable(${THIS})
target_sources(
     ${THIS}
     PRIVATE "main.cpp"
)
target_link_libraries(
     ${THIS}
     PRIVATE Boost::random
)