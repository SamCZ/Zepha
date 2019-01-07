cmake_minimum_required(VERSION 3.12)
project(zeus)

set(CMAKE_CXX_STANDARD 14)

include_directories(lib/glew/include/GL)
include_directories(lib/glfw_linux/include)
include_directories(lib/glm)
include_directories(lib/stb_image)
include_directories(lib/cute)
include_directories(lib/sol)
include_directories(lib/asio-1.12.2/include)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

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
        zeus/game/GameScene.cpp
        zeus/game/GameScene.h
        zeus/engine/graphics/Renderer.cpp
        zeus/engine/graphics/Renderer.h
        zeus/lua_api/LuaParser.cpp
        zeus/lua_api/LuaParser.h
        zeus/lua_api/l_register_block.cpp
        zeus/lua_api/l_register_block.h
        zeus/engine/graphics/TextBuilder.cpp
        zeus/engine/graphics/TextBuilder.h
        zeus/engine/graphics/TextBuilder.cpp
        zeus/engine/graphics/HudText.cpp
        zeus/engine/graphics/HudText.h
        zeus/game/gui/DebugGui.cpp
        zeus/game/gui/DebugGui.h
        zeus/game/world/Player.cpp
        zeus/game/world/Player.h
        zeus/engine/Ray.cpp
        zeus/engine/Ray.h
        zeus/lua_api/l_register_blockmodel.cpp
        zeus/lua_api/l_register_blockmodel.h
        zeus/lua_api/LuaApi.cpp
        zeus/lua_api/LuaApi.h
        zeus/client/Client.cpp
        zeus/client/Client.h
        zeus/client/SceneManager.cpp
        zeus/client/SceneManager.h
        zeus/client/Scene.h
        zeus/client/ClientState.h)

target_link_libraries(zeus ${OPENGL_gl_LIBRARY} glfw libGLEW.so pthread lua dl)
