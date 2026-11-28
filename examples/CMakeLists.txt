cmake_minimum_required(VERSION 4.4.5)
project(examples)

include(FetchContent)

FetchContent_Declare(
    asio
	GIT_REPOSITORY 	https://github.com/chriskohlhoff/asio.git
	GIT_TAG asio-1-38-2
	GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(asio)

add_executable(rockpaperscissor)

add_subdirectory(_deps)
add_subdirectory(src)

target_link_libraries(rockpaperscissor
    PRIVATE asio
)