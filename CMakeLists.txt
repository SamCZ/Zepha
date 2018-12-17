cmake_minimum_required(VERSION 3.12)
project(src)

set(CMAKE_CXX_STANDARD 14)

include_directories(Libraries/glew/include/GL)
include_directories(Libraries/glfw_linux/include)
include_directories(Libraries/glm)
include_directories(Libraries/stb_image)
include_directories(Libraries/cute)
#include_directories(Libraries/boost_1_68_0)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
#find_package(Boost COMPONENTS system thread)

link_directories(Libraries/glew/lib)

add_executable(src src/Main.cpp src/engine/graphics/Mesh.cpp src/engine/graphics/Mesh.h src/engine/PerlinNoise.cpp src/engine/PerlinNoise.h src/engine/Entity.cpp src/engine/Entity.h src/engine/graphics/Shader.cpp src/engine/graphics/Shader.h src/engine/Window.cpp src/engine/Window.h src/engine/Camera.cpp src/engine/Camera.h src/engine/graphics/Texture.cpp src/engine/graphics/Texture.h src/mesh/MeshGenerator.cpp src/mesh/MeshGenerator.h src/engine/Timer.cpp src/engine/Timer.h src/blocks/BlockAtlas.cpp src/blocks/BlockAtlas.h src/blocks/BlockDef.cpp src/blocks/BlockDef.h src/mesh/MeshPart.cpp src/mesh/MeshPart.h src/mesh/MeshMod.h src/mesh/Vertex.cpp src/mesh/Vertex.h src/mesh/BlockModel.cpp src/mesh/BlockModel.h src/engine/TextureAtlas.cpp src/engine/TextureAtlas.h src/world/World.cpp src/world/World.h src/world/BlockChunk.cpp src/world/BlockChunk.h src/engine/helpers/ArrayTrans3D.h src/world/MeshChunk.cpp src/world/MeshChunk.h)

target_link_libraries(src
        ${OPENGL_gl_LIBRARY}
        glfw
        libGLEW.so
        pthread
        lua
        dl)