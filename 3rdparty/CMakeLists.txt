include(FetchContent)
FetchContent_Declare(celero
     GIT_REPOSITORY "https://github.com/DigitalInBlue/Celero.git"
     GIT_TAG "v2.8.5"
)

if(BUILD_BENCHMARKS)
     FetchContent_MakeAvailable(celero)
endif()
