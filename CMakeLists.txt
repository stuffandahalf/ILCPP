cmake_minimum_required(VERSION 3.1)
project(ILCPP C)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads)

add_executable(ilcpp
	src/main.c
)

target_compile_definitions(ilcpp PRIVATE _POSIX_C_SOURCE=200112L)

if(CMAKE_USE_PTHREADS_INIT)
	target_compile_definitions(ilcpp PRIVATE HAVE_PTHREADS=1)
	target_link_libraries(ilcpp Threads::Threads)
endif()

set_target_properties(ilcpp
	PROPERTIES
		C_STANDARD 90
		C_STANDARD_REQUIRED YES
		C_EXTENSIONS NO
)

