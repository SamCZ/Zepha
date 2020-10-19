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
    find_library(ASSIMP_LIB assimp)
    target_link_libraries(${MAIN_EXEC_NAME} assimp)
endif()

# Link Lua 5.3.5
find_library(LUA_LIB NAMES lua lua5.1 lua5.3)
target_link_libraries(${MAIN_EXEC_NAME} ${LUA_LIB})

# Link Noise
target_link_libraries(${MAIN_EXEC_NAME} ${NOISE_LIB})

# Link PThread Dynamically
target_link_libraries (${MAIN_EXEC_NAME} ${PTHREAD_LIB})

# Link Z Dynamically
target_link_libraries (${MAIN_EXEC_NAME} z)