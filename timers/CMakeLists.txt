cmake_minimum_required(VERSION 4.4.5)
project(timers)

include(FetchContent)

set(FETCHCONTENT_BASE_DIR ${PROJECT_SOURCE_DIR}/external/asio)

FetchContent_Declare(
	asio
	GIT_REPOSITORY 	https://github.com/chriskohlhoff/asio.git
	GIT_TAG asio-1-38-2
	GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(asio)
add_executable(timer_1)
add_executable(timer_2)
add_executable(timer_3)
add_executable(timer_4)
add_executable(timer_5)

add_subdirectory(external)
add_subdirectory(src)

target_link_libraries(timer_1
PRIVATE asio
)

target_link_libraries(timer_2
PRIVATE asio
)

target_link_libraries(timer_3
PRIVATE asio
)

target_link_libraries(timer_4
PRIVATE asio
)

target_link_libraries(timer_5
PRIVATE asio
)