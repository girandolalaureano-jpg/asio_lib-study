cmake_minimum_required(VERSION 4.4.5)
project(daytimes)


include(FetchContent)

set(FETCHCONTENT_BASE_DIR ${PROJECT_SOURCE_DIR}/external/asio)

FetchContent_Declare(
	asio
	GIT_REPOSITORY 	https://github.com/chriskohlhoff/asio.git
	GIT_TAG asio-1-38-2
	GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(asio)

add_executable(daytime1)
add_executable(daytime2)
add_executable(daytime3)

add_subdirectory(external)
add_subdirectory(src)

target_link_libraries(daytime1
PRIVATE asio
)

target_link_libraries(daytime2
PRIVATE asio
)

target_link_libraries(daytime3
PRIVATE asio
)
