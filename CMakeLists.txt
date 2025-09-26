cmake_minimum_required(VERSION 3.29)
project(picturegobrrrrr)
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG 11.0.2
)
FetchContent_MakeAvailable(fmt)

include(FetchContent)
FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.0
)
FetchContent_MakeAvailable(SFML)

add_executable(picturegobrrrrr main.cpp)
target_link_libraries(picturegobrrrrr fmt SFML::Graphics SFML::Audio SFML::Network)