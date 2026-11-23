add_library(asio INTERFACE)
find_package(Threads REQUIRED)


target_include_directories(asio INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/asio/asio-src/include)
target_compile_definitions(asio INTERFACE ASIO_STANDALONE)
target_link_libraries(asio INTERFACE Threads::Threads)

if(WIN32)
	target_compile_definitions(asio INTERFACE _WIN32_WINNT=0x0A00 WIN32_LEAN_AND_MEAN)
	target_link_libraries(asio INTERFACE ws2_32 mswsock)
endif()