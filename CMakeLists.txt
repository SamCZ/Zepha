cmake_minimum_required(VERSION 3.19)

cmake_policy(SET CMP0077 NEW) # set() overrides option()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set warnings
if ((CMAKE_CXX_COMPILER_ID STREQUAL "GNU") OR (CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))
    add_compile_options(
            -Werror -Wall -Wextra -pedantic -pedantic-errors
            -Wnon-virtual-dtor -Wmisleading-indentation -Wlogical-op -Wnull-dereference
            -Wno-unused-parameter -Wno-reorder -Wno-sign-compare)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/permissive /W4 /w14640)
endif()

set(PROJECT_NAME "Zepha")
set(MAIN_EXEC_NAME "Zepha")
# set(TEST_EXEC_NAME "ZephaTest")

project(${PROJECT_NAME})

add_subdirectory(lib)
set(BUILD_SHARED_LIBS OFF)

add_subdirectory(src)

add_executable(${MAIN_EXEC_NAME} src/Main.cpp)
target_link_libraries(${MAIN_EXEC_NAME} Zepha_Core)