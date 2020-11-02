cmake_minimum_required (VERSION 3.12 FATAL_ERROR)
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

find_path(GLEW_HEADERS GL/glew.h)
find_path(GLFW_HEADERS GLFW/glfw3.h)
find_path(LUA_HEADERS lua.hpp
        /usr/include/lua5.3
        /usr/local/include/lua5.3)
find_path(ASSIMP_HEADERS assimp/Importer.hpp)
find_path(ENET_HEADERS enet/enet.h)
find_path(NOISE_HEADERS NAMES libnoise/noise.h libnoise/module/modulebase.h)
find_path(GLM_HEADERS glm/glm.hpp)
find_path(PTHREAD_HEADERS pthread.h)

find_library(ENET_LIB enet)
find_library(NOISE_LIB NAMES libnoise noise)

if (WIN32)
    find_library(PTHREAD_LIB pthreadVC3)
else()
    find_library(PTHREAD_LIB pthread)
endif()

include_directories(
    ${GLM_HEADERS}
    ${GLEW_HEADERS}
    ${LUA_HEADERS}
    ${ASSIMP_HEADERS}
    ${ENET_HEADERS}
    ${NOISE_HEADERS}
    ${PTHREAD_HEADERS}
    lib/catch2/include
    lib/gzip/include
    lib/cute_files/include
    lib/stb_image/include
    lib/json/include
    lib/sol/include
)

add_subdirectory(src)
add_executable(${MAIN_EXEC_NAME} src/Main.cpp)
target_link_libraries(${MAIN_EXEC_NAME} Zepha_Core)

target_include_directories(${MAIN_EXEC_NAME} PRIVATE ${GLFW_HEADERS})

# Enet
target_link_libraries(${MAIN_EXEC_NAME} ${ENET_LIB})

# Find and Link OpenGL
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

target_link_libraries(${MAIN_EXEC_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

# Build GLFW
if (WIN32)
    find_library(GLFW_LIB glfw3dll)
else()
    find_library(GLFW_LIB NAMES GLFW glfw glfw3)
endif()
target_link_libraries(${MAIN_EXEC_NAME} ${GLFW_LIB})

# Link GLEW
if (WIN32)
    find_library(GLEW_LIB glew32)
else()
    find_library(GLEW_LIB NAMES GLEW glew3)
endif()
target_link_libraries(${MAIN_EXEC_NAME} ${GLEW_LIB})

# Build and Link Assimp

if (WIN32)
    find_library(ASSIMP_LIB assimp-vc142-mt)
else()
    set(BUILD_SHARED_LIBS OFF)
    set(ASSIMP_NO_EXPORT ON)
    set(ASSIMP_BUILD_TESTS OFF)
    set(ASSIMP_BUILD_ASSIMP_TOOLS OFF)

    set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF)
    set(ASSIMP_BUILD_B3D_IMPORTER ON)
    set(ASSIMP_BUILD_X3D_IMPORTER ON) # Doesn't compile if not defined

    add_subdirectory(lib/assimp)
    target_compile_options(assimp PRIVATE -w)
    target_link_libraries(${MAIN_EXEC_NAME} assimp)
endif()

# Link Lua 5.3.5
find_library(LUA_LIB NAMES lua lua5.3)
target_link_libraries(${MAIN_EXEC_NAME} ${LUA_LIB})

# Link Noise
target_link_libraries(${MAIN_EXEC_NAME} ${NOISE_LIB})

# Link PThread Dynamically
target_link_libraries (${MAIN_EXEC_NAME} ${PTHREAD_LIB})

# Link Z Dynamically
target_link_libraries (${MAIN_EXEC_NAME} z)

# Fix Win32 networking
if(WIN32)
target_link_libraries(${MAIN_EXEC_NAME} winmm ws2_32)
endif()

# Test Build
# add_subdirectory(test)
# add_executable(${TEST_EXEC_NAME} test/Main.cpp)
# target_link_libraries(${TEST_EXEC_NAME} Zepha_Core)
# target_link_libraries(${TEST_EXEC_NAME} Zepha_Test)

# target_include_directories(${TEST_EXEC_NAME} PRIVATE ${GLFW_HEADERS})
# target_link_libraries(${TEST_EXEC_NAME} ${LUA_LIB})
# target_link_libraries (${TEST_EXEC_NAME} z)
# target_link_libraries(${TEST_EXEC_NAME} ${ENET_LIB})