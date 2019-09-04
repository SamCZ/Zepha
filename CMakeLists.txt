cmake_minimum_required (VERSION 3.12 FATAL_ERROR)
set (CMAKE_CXX_STANDARD 14)

# Define Variables
set (PROJECT_NAME "Zeus")
set (MAIN_EXEC_NAME "ZeusApp")
set (TEST_EXEC_NAME "ZeusTest")

project (${PROJECT_NAME})

# Include Header only Libraries
set (HEADER_LIBRARIES
        lib/header/catch2/include      # Catch2 unit testing library.
        lib/header/gzip/include        # GZip Compression header library.
        lib/header/glm/include         # OpenGL Mathematics Library.
        lib/header/sol2/include        # Sol2 Lua Parser library.
        lib/header/cute_files/include  # Cute file traversal header library.
        lib/header/stb_image/include   # STB Image processor header library.
        lib/header/json/include        # JSON header library.
)

include_directories (${HEADER_LIBRARIES})

# Include dynamic library headers
set (DYNAMIC_LIBRARY_HEADERS
        lib/static/glew/include        # GLEW headers.
        lib/static/glfw/include        # GLFW headers.

        lib/dynamic/enet/include       # ENet headers.
        lib/dynamic/noise/include      # Noise headers.
        lib/dynamic/lua5.3/include     # Lua headers.
        lib/dynamic/assimp/include     # Assimp headers.
)

include_directories (${DYNAMIC_LIBRARY_HEADERS})

# Add source files to the executable as a library
add_subdirectory (src)
add_executable (${MAIN_EXEC_NAME} src/Main.cpp src/StartGame.h)
target_link_libraries (${MAIN_EXEC_NAME} zeusCore)

# Include OpenGL
find_package (OpenGL REQUIRED)
target_link_libraries(${MAIN_EXEC_NAME} ${OPENGL_gl_LIBRARY})

# GLFW Static Library
set (GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set (GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set (GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory (lib/static/glfw)
target_link_libraries (${MAIN_EXEC_NAME} glfw)

# Glew Static Library
add_subdirectory (lib/static/glew/build/cmake)
target_link_libraries (${MAIN_EXEC_NAME} glew)

# Include the other dynamic libraries - End goal would be to build these at compile time
set (ZEUS_LIBRARIES pthread lua5.3 z enet noise assimp)

target_link_libraries (${MAIN_EXEC_NAME} ${ZEUS_LIBRARIES})

# Test Build
#add_subdirectory(test)
#add_executable(${TEST_EXEC_NAME} test/Main.cpp)
#
#target_link_libraries(${TEST_EXEC_NAME} zeusCore)
#target_link_libraries(${TEST_EXEC_NAME} zeusTest)
