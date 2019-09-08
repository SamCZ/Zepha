cmake_minimum_required (VERSION 3.12 FATAL_ERROR)
set (CMAKE_CXX_STANDARD 14)

set (PROJECT_NAME "Zepha")
set (MAIN_EXEC_NAME "Zepha")
set (TEST_EXEC_NAME "ZephaTest")

project (${PROJECT_NAME})

include_directories(
    # Include Header Only Libraries
    lib/header/catch2/include      # Catch2 unit testing library.
    lib/header/gzip/include        # GZip Compression header library.
    lib/header/glm/include         # OpenGL Mathematics Library.
    lib/header/sol2/include        # Sol2 Lua Parser library.
    lib/header/cute_files/include  # Cute file traversal header library.
    lib/header/stb_image/include   # STB Image processor header library.
    lib/header/json/include        # JSON header library.

    # Include Static Library Headers
    lib/static/glew/include        # GLEW headers.
    lib/static/glfw/include        # GLFW headers.
    lib/static/assimp/include      # Assimp headers.
    lib/static/lua/install/include # Lua headers.
    lib/static/enet/include        # ENet headers.
    lib/static/noise/include       # Noise headers.
)

add_subdirectory (src)
add_executable (${MAIN_EXEC_NAME} src/Main.cpp)
target_link_libraries (${MAIN_EXEC_NAME} Zepha_Core)

# Load Libraries
include(${CMAKE_CURRENT_SOURCE_DIR}/LoadLibs.cmake)

# Test Build
#add_subdirectory(test)
#add_executable(${TEST_EXEC_NAME} test/Main.cpp)
#
#target_link_libraries(${TEST_EXEC_NAME} Zepha_Core)
#target_link_libraries(${TEST_EXEC_NAME} zeusTest)
