cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
set (CMAKE_CXX_STANDARD 14)
set (PROJECT_NAME "Zeus")

#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined")

project (${PROJECT_NAME})

#Include all of the library headers
include_directories (
        lib/catch2/include      # Catch2 unit testing library.
        lib/cute_files/include  # Cute file traversal header library.
        lib/enet/include        # ENet networking library.
        lib/glew/include        # GLEW library.
        lib/glfw/include        # GLFW library.
        lib/glm/include         # OpenGL Mathematics Library.
        lib/gzip/include        # GZip Compression header library
        lib/noise/include       # LibNoise library.
        lib/sol2/include        # Sol2 Lua Parser library
        lib/stb_image/include   # stb Image processor header library.

        /usr/include/lua5.3     # Lua Headers
)

#Include all of the dynamic libraries
set (ZEUS_LIBRARIES
        ${OPENGL_gl_LIBRARY}    #opengl
        libGLEW.so              #glew
        pthread                 #std::thread
        lua5.3                  #sol2
        dl                      #gzip
        z                       #zlib
        enet                    #enet
        noise                   #libnoise
)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

link_directories(lib/glew/lib)

#Configure Main Executable
add_subdirectory(src)

set(MAIN_EXEC_NAME "ZeusApp")

add_executable(${MAIN_EXEC_NAME} src/Main.cpp)
target_link_libraries(${MAIN_EXEC_NAME} zeusCore)
target_link_libraries(${MAIN_EXEC_NAME} ${ZEUS_LIBRARIES})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(lib/glfw)
target_link_libraries(ZeusApp glfw)

#Configure Test Executable
add_subdirectory(test)

set(TEST_EXEC_NAME "ZeusTest")

add_executable(${TEST_EXEC_NAME} test/Main.cpp)
target_link_libraries(${TEST_EXEC_NAME} zeusCore)
target_link_libraries(${TEST_EXEC_NAME} zeusTest)
target_link_libraries(${TEST_EXEC_NAME} ${ZEUS_LIBRARIES})
