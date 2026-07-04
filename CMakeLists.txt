cmake_minimum_required(VERSION 3.16)
project(asynx VERSION 0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Source files
set(SOURCES
	src/common.c
	src/thread.c
	src/threadpool.c
)

# Add a quick test executable (e.g., if you create a src/main.c)
# add_executable(test_runner src/main.c)
# target_link_libraries(test_runner PRIVATE asynx)
# add_executable(asynx ${SOURCES})
# 1. Changed from add_executable to add_library
add_library(asynx STATIC ${SOURCES})
target_include_directories(asynx PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_compile_definitions(asynx PRIVATE _XOPEN_SOURCE=700)

# Compiler warnings
if(MSVC)
	target_compile_options(asynx PRIVATE /W4)
else()
	target_compile_options(asynx PRIVATE -Wall -Wextra -Wpedantic)
endif()


