cmake_minimum_required(VERSION 3.19)

cmake_policy(SET CMP0077 NEW) # set() overrides option()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(ZP_BUILD_SHARED_LIBS ON)
else()
    set(ZP_BUILD_SHARED_LIBS OFF)
endif()

if(ZP_BUILD_SHARED_LIBS)
    set(BUILD_SHARED_LIBS ON)
    set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()

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

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

add_subdirectory(lib)

file(GLOB_RECURSE SOURCE_FILES src/*.hpp src/*.cpp)

add_executable(${MAIN_EXEC_NAME} ${SOURCE_FILES})

target_include_directories(${MAIN_EXEC_NAME} PUBLIC src)

target_link_libraries(${MAIN_EXEC_NAME} PUBLIC glfw glm enet glad assimp FastNoise nothings-stb gzip cute_headers nlohmann_json stdc++fs)
target_link_libraries(${MAIN_EXEC_NAME} PUBLIC lua sol2)

if(WIN32)
    target_link_libraries(${MAIN_EXEC_NAME} PUBLIC opengl32 gdi32)
endif()

if(WIN32)
    foreach(DLL ${ZEPHA_DLL_TO_COPY})
        add_custom_command(TARGET ${MAIN_EXEC_NAME} POST_BUILD
                COMMAND "${CMAKE_COMMAND}" -E copy
                ${DLL}
                ${CMAKE_CURRENT_BINARY_DIR}
                COMMENT "Copying dll ${DLL} to output directory")
    endforeach(DLL)
endif(WIN32)