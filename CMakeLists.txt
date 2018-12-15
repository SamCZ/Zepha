cmake_minimum_required(VERSION 3.12)
project(GlProject)

set(CMAKE_CXX_STANDARD 14)

include_directories(Libraries/glew/include/GL)
include_directories(Libraries/glfw_linux/include)
include_directories(Libraries/glm)
include_directories(Libraries/stb_image)
include_directories(Libraries/cute)
include_directories(Libraries/boost_1_68_0)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Boost COMPONENTS system thread)

link_directories(Libraries/glew/lib)

add_executable(GlProject GlProject/Main.cpp GlProject/engine/graphics/Mesh.cpp GlProject/engine/graphics/Mesh.h GlProject/engine/PerlinNoise.cpp GlProject/engine/PerlinNoise.h GlProject/engine/Entity.cpp GlProject/engine/Entity.h GlProject/engine/graphics/Shader.cpp GlProject/engine/graphics/Shader.h GlProject/engine/Window.cpp GlProject/engine/Window.h GlProject/engine/Camera.cpp GlProject/engine/Camera.h GlProject/engine/graphics/Texture.cpp GlProject/engine/graphics/Texture.h GlProject/mesh/MeshGenerator.cpp GlProject/mesh/MeshGenerator.h GlProject/engine/Timer.cpp GlProject/engine/Timer.h GlProject/blocks/BlockAtlas.cpp GlProject/blocks/BlockAtlas.h GlProject/blocks/BlockDef.cpp GlProject/blocks/BlockDef.h GlProject/mesh/MeshPart.cpp GlProject/mesh/MeshPart.h GlProject/mesh/MeshMod.h GlProject/mesh/Vertex.cpp GlProject/mesh/Vertex.h GlProject/mesh/BlockModel.cpp GlProject/mesh/BlockModel.h GlProject/engine/TextureAtlas.cpp GlProject/engine/TextureAtlas.h GlProject/UDP.cpp GlProject/UDP.h)

target_link_libraries(GlProject ${OPENGL_gl_LIBRARY} glfw libGLEW.so pthread)