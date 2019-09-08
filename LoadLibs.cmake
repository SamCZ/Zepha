# Find and Link OpenGL
find_package (OpenGL REQUIRED)
target_link_libraries(${MAIN_EXEC_NAME} ${OPENGL_gl_LIBRARY})

# Build and Link GLFW
set (GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set (GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set (GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory (lib/static/glfw)
target_link_libraries (${MAIN_EXEC_NAME} glfw)
include_directories(lib/static/glfw/src)

# Link Glew
target_link_libraries(${MAIN_EXEC_NAME} ${CMAKE_SOURCE_DIR}/lib/static/glew/libGLEW.a)

# Build and Link Assimp
set(BUILD_SHARED_LIBS OFF)
set(ASSIMP_NO_EXPORT ON)
set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF)

set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF)
set(ASSIMP_BUILD_B3D_IMPORTER ON)
set(ASSIMP_BUILD_X3D_IMPORTER ON) # Doesn't compile if not defined

add_subdirectory(lib/static/assimp)
target_compile_options(assimp PRIVATE -w)
target_link_libraries(${MAIN_EXEC_NAME} assimp)

# Link Lua 5.3.5
target_link_libraries(${MAIN_EXEC_NAME} ${CMAKE_SOURCE_DIR}/lib/static/lua/install/lib/liblua.a)

# Enet
target_link_libraries(${MAIN_EXEC_NAME} ${CMAKE_SOURCE_DIR}/lib/static/enet/.libs/libenet.a)

# Link Noise
target_link_libraries(${MAIN_EXEC_NAME} ${CMAKE_SOURCE_DIR}/lib/static/noise/build/src/libnoise.a)

# Link PThread Dynamically
target_link_libraries (${MAIN_EXEC_NAME} pthread)

# Link Z Dynamically
target_link_libraries (${MAIN_EXEC_NAME} z)