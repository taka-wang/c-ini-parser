cmake_minimum_required(VERSION 2.8)
project(IniParser C)

# cflag
set(EXT_C_FLAGS "-W -Wall -Wno-format-security -O3 -std=c99")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXT_C_FLAGS}")


set(CMAKE_VERBOSE_MAKEFILE ON)
add_subdirectory(src)

# Flag
option (ENABLE_EXAMPLE "build test in examples folder" ON)

if (ENABLE_EXAMPLE)
    add_subdirectory(examples)
endif (ENABLE_EXAMPLE)
