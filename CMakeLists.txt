cmake_minimum_required(VERSION 3.26)
project(saras C)

set(CMAKE_C_STANDARD 99)

add_executable(saras library.c)

# Assume SDL2 library is installed in your system
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Link against SDL2
target_link_libraries(saras ${SDL2_LIBRARIES})

