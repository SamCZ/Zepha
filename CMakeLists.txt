cmake_minimum_required (VERSION 3.12 FATAL_ERROR)
set (CMAKE_CXX_STANDARD 17)

set (PROJECT_NAME "Zepha")
set (MAIN_EXEC_NAME "Zepha")
set (TEST_EXEC_NAME "ZephaTest")

project (${PROJECT_NAME})

find_path(GLEW_HEADERS GL/glew.h)
find_path(GLFW_HEADERS GLFW/glfw3.h)
find_path(LUA_HEADERS lua.hpp
        /usr/include/lua5.1
        /usr/local/include/lua5.1)
find_path(ASSIMP_HEADERS assimp/Importer.hpp)
find_path(ENET_HEADERS enet/enet.h)
find_path(NOISE_HEADERS noise/noise.h)
find_path(GLM_HEADERS glm/glm.hpp)
find_path(PTHREAD_HEADERS pthread.h)

if (WIN32)
find_library(GLEW_LIB glew32)
else()
set(GLEW_LIB ${CMAKE_SOURCE_DIR}/lib/static/glew/libGLEW.a)
endif()

if (WIN32)
find_library(GLFW_LIB glfw3dll)
else()
find_library(GLFW_LIB glfw3)
endif()

find_library(LUA_LIB NAMES lua lua5.1 PATHS
        /usr/lib/x86_64-linux-gnu
        /usr/lib
        /usr/local/lib)

if (WIN32)
find_library(ASSIMP_LIB assimp-vc142-mt)
else()
find_library(ASSIMP_LIB assimp)
endif()

find_library(ENET_LIB enet)
find_library(NOISE_LIB NAMES libnoise noise noise-static)

if (WIN32)
find_library(PTHREAD_LIB pthreadVC3)
else()
find_library(PTHREAD_LIB pthread)
endif()

include_directories(
    # Include Header Only Libraries
    lib/header/catch2/include      # Catch2 unit testing library.
    lib/header/gzip/include        # GZip Compression header library.
    ${GLM_HEADERS}                 # OpenGL Mathematics Library.
    lib/header/sol2/include        # Sol2 Lua Parser library.
    lib/header/cute_files/include  # Cute file traversal header library.
    lib/header/stb_image/include   # STB Image processor header library.
    lib/header/json/include        # JSON header library.

    # Include Static Library Headers
    ${GLEW_HEADERS}
    ${LUA_HEADERS}
    ${ASSIMP_HEADERS}
    ${ENET_HEADERS}
    ${NOISE_HEADERS}
    ${PTHREAD_HEADERS}
)


add_subdirectory (src)
add_executable (${MAIN_EXEC_NAME} src/Main.cpp)
target_link_libraries (${MAIN_EXEC_NAME} Zepha_Core)

target_include_directories(${MAIN_EXEC_NAME} PRIVATE ${GLFW_HEADERS})
# Load Libraries
include(${CMAKE_CURRENT_SOURCE_DIR}/LoadLibs.cmake)

# Fix Win32 networking
if(WIN32)
target_link_libraries(${MAIN_EXEC_NAME} winmm ws2_32)
endif()

# Enable Safeties
target_compile_definitions(${MAIN_EXEC_NAME} PUBLIC SOL_ALL_SAFETIES_ON)

# Test Build
#add_subdirectory(test)
#add_executable(${TEST_EXEC_NAME} test/Main.cpp)
#
#target_link_libraries(${TEST_EXEC_NAME} Zepha_Core)
#target_link_libraries(${TEST_EXEC_NAME} zeusTest)
