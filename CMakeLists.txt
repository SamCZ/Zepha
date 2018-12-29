cmake_minimum_required(VERSION 3.12)
project(zeus)

set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_FLAGS "-s -O3")

include_directories(lib/glew/include/GL)
include_directories(lib/glfw_linux/include)
include_directories(lib/glm)
include_directories(lib/stb_image)
include_directories(lib/cute)
include_directories(lib/sol)
#include_directories(Libraries/boost_1_68_0)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
#find_package(Boost COMPONENTS system thread)

link_directories(lib/glew/lib)

add_executable(zeus
        zeus/Main.cpp
        zeus/engine/graphics/Mesh.cpp
        zeus/engine/graphics/Mesh.h
        zeus/engine/PerlinNoise.cpp
        zeus/engine/PerlinNoise.h
        zeus/engine/Entity.cpp
        zeus/engine/Entity.h
        zeus/engine/graphics/Shader.cpp
        zeus/engine/graphics/Shader.h
        zeus/engine/Window.cpp
        zeus/engine/Window.h
        zeus/engine/Camera.cpp
        zeus/engine/Camera.h
        zeus/engine/graphics/Texture.cpp
        zeus/engine/graphics/Texture.h
        zeus/mesh/MeshGenerator.cpp
        zeus/mesh/MeshGenerator.h
        zeus/engine/Timer.cpp
        zeus/engine/Timer.h
        zeus/blocks/BlockAtlas.cpp
        zeus/blocks/BlockAtlas.h
        zeus/blocks/BlockDef.cpp
        zeus/blocks/BlockDef.h
        zeus/mesh/MeshPart.cpp
        zeus/mesh/MeshPart.h
        zeus/mesh/MeshMod.h
        zeus/mesh/Vertex.cpp
        zeus/mesh/Vertex.h
        zeus/mesh/BlockModel.cpp
        zeus/mesh/BlockModel.h
        zeus/engine/TextureAtlas.cpp
        zeus/engine/TextureAtlas.h
        zeus/game/world/World.cpp
        zeus/game/world/World.h
        zeus/blocks/BlockChunk.cpp
        zeus/blocks/BlockChunk.h
        zeus/engine/helpers/ArrayTrans3D.h
        zeus/mesh/MeshChunk.cpp
        zeus/mesh/MeshChunk.h
        zeus/game/GameInstance.cpp
        zeus/game/GameInstance.h
        zeus/engine/graphics/Renderer.cpp
        zeus/engine/graphics/Renderer.h
        zeus/lua_api/LuaParser.cpp
        zeus/lua_api/LuaParser.h
        zeus/lua_api/LRegisterBlock.cpp
        zeus/lua_api/LRegisterBlock.h
        zeus/engine/graphics/TextBuilder.cpp
        zeus/engine/graphics/TextBuilder.h
        zeus/engine/graphics/TextBuilder.cpp
        zeus/engine/graphics/HudText.cpp
        zeus/engine/graphics/HudText.h zeus/game/gui/DebugGui.cpp zeus/gui/DebugGui.h)

target_link_libraries(zeus ${OPENGL_gl_LIBRARY} glfw libGLEW.so pthread lua dl)