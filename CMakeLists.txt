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
include_directories(lib/gzip-hpp-master/include)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

link_directories(lib/glew/lib)

add_executable(zeus
        zeus/Main.cpp
        zeus/client/engine/graphics/Mesh.cpp
        zeus/client/engine/graphics/Mesh.h
        zeus/generic/helpers/PerlinNoise.cpp
        zeus/generic/helpers/PerlinNoise.h
        zeus/client/engine/Entity.cpp
        zeus/client/engine/Entity.h
        zeus/client/engine/graphics/Shader.cpp
        zeus/client/engine/graphics/Shader.h
        zeus/client/engine/Window.cpp
        zeus/client/engine/Window.h
        zeus/client/engine/Camera.cpp
        zeus/client/engine/Camera.h
        zeus/client/engine/graphics/Texture.cpp
        zeus/client/engine/graphics/Texture.h
        zeus/client/graphics/mesh/MeshGenerator.cpp
        zeus/client/graphics/mesh/MeshGenerator.h
        zeus/client/engine/Timer.cpp
        zeus/client/engine/Timer.h
        zeus/generic/blocks/BlockAtlas.cpp
        zeus/generic/blocks/BlockAtlas.h
        zeus/generic/blocks/BlockDef.cpp
        zeus/generic/blocks/BlockDef.h
        zeus/generic/blocks/MeshPart.cpp
        zeus/generic/blocks/MeshPart.h
        zeus/generic/blocks/MeshMod.h
        zeus/client/graphics/mesh/Vertex.cpp
        zeus/client/graphics/mesh/Vertex.h
        zeus/generic/blocks/BlockModel.cpp
        zeus/generic/blocks/BlockModel.h
        zeus/generic/blocks/TextureAtlas.cpp
        zeus/generic/blocks/TextureAtlas.h
        zeus/client/gameworld/World.cpp
        zeus/client/gameworld/World.h
        zeus/generic/blocks/BlockChunk.cpp
        zeus/generic/blocks/BlockChunk.h
        zeus/generic/helpers/ArrayTrans3D.h
        zeus/client/graphics/mesh/MeshChunk.cpp
        zeus/client/graphics/mesh/MeshChunk.h
        zeus/client/scene/GameScene.cpp
        zeus/client/scene/GameScene.h
        zeus/client/engine/graphics/Renderer.cpp
        zeus/client/engine/graphics/Renderer.h
        zeus/client/lua/LuaParser.cpp
        zeus/client/lua/LuaParser.h
        zeus/client/lua/l_register_block.cpp
        zeus/client/lua/l_register_block.h
        zeus/client/engine/graphics/TextBuilder.cpp
        zeus/client/engine/graphics/TextBuilder.h
        zeus/client/engine/graphics/TextBuilder.cpp
        zeus/client/engine/graphics/HudText.cpp
        zeus/client/engine/graphics/HudText.h
        zeus/client/graphics/gui/DebugGui.cpp
        zeus/client/graphics/gui/DebugGui.h
        zeus/client/gameworld/Player.cpp
        zeus/client/gameworld/Player.h
        zeus/client/engine/Ray.cpp
        zeus/client/engine/Ray.h
        zeus/client/lua/l_register_blockmodel.cpp
        zeus/client/lua/l_register_blockmodel.h
        zeus/client/lua/LuaApi.cpp
        zeus/client/lua/LuaApi.h
        zeus/client/Client.cpp
        zeus/client/Client.h
        zeus/client/engine/scene/SceneManager.cpp
        zeus/client/engine/scene/SceneManager.h
        zeus/client/engine/scene/Scene.h
        zeus/client/ClientState.h
        zeus/client/scene/MenuScene.cpp
        zeus/client/scene/MenuScene.h
        zeus/server/Server.cpp
        zeus/server/Server.h
        zeus/server/network/ServerClient.cpp
        zeus/server/network/ServerClient.h
        zeus/generic/network/Packet.cpp
        zeus/generic/network/Packet.h
        zeus/server/ServerPlayer.cpp
        zeus/server/ServerPlayer.h
        zeus/client/network/ServerConnection.cpp
        zeus/client/network/ServerConnection.h
        zeus/client/gameworld/WorldThreadDefs.cpp
        zeus/client/gameworld/WorldThreadDefs.h
        zeus/server/network/ConnMan.cpp
        zeus/server/network/ConnMan.h zeus/generic/gen/MapGen.cpp zeus/generic/gen/MapGen.h)

target_link_libraries(zeus ${OPENGL_gl_LIBRARY} glfw libGLEW.so pthread lua dl z)
