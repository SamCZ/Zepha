# Enet
target_link_libraries(${MAIN_EXEC_NAME} ${ENET_LIB})

# Find and Link OpenGL
find_package (OpenGL REQUIRED)
target_link_libraries(${MAIN_EXEC_NAME} ${OPENGL_gl_LIBRARY})

# Build GLFW
if (WIN32)
    find_library(GLFW_LIB glfw3dll)
else()
    set (GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set (GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set (GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

    add_subdirectory (lib/static/glfw)
    target_link_libraries (${MAIN_EXEC_NAME} glfw)
    include_directories(lib/static/glfw/src)
endif()

# Link GLFW
if (WIN32)
    find_library(GLEW_LIB glew32)
else()
    set(GLEW_LIB ${CMAKE_SOURCE_DIR}/lib/static/glew/libGLEW.a)
endif()

# Link Glew
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

    add_subdirectory(lib/static/assimp)
    target_compile_options(assimp PRIVATE -w)
    target_link_libraries(${MAIN_EXEC_NAME} assimp)
endif()

# Link Lua 5.3.5
target_link_libraries(${MAIN_EXEC_NAME} ${LUA_LIB})

# Link Noise
target_link_libraries(${MAIN_EXEC_NAME} ${NOISE_LIB})

# Link PThread Dynamically
target_link_libraries (${MAIN_EXEC_NAME} ${PTHREAD_LIB})

# Link Z Dynamically
target_link_libraries (${MAIN_EXEC_NAME} z)