cmake_minimum_required(VERSION 3.12)
project(GlProject)

set(CMAKE_CXX_STANDARD 14)

include_directories(GlProject/Libraries/glew/include/GL)
include_directories(GlProject/Libraries/glfw_linux/include)
include_directories(GlProject/Libraries/glm)
include_directories(GlProject/Libraries/stb_image)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

link_directories(GlProject/Libraries/glew/lib)

add_executable(GlProject
        GlProject/Main.cpp GlProject/Mesh.cpp GlProject/Mesh.h GlProject/Entity.cpp GlProject/Entity.h GlProject/Shader.cpp GlProject/Shader.h GlProject/Window.cpp GlProject/Window.h GlProject/Camera.cpp GlProject/Camera.h GlProject/Texture.cpp GlProject/Texture.h GlProject/MeshGenerator.cpp GlProject/MeshGenerator.h GlProject/MeshData.cpp GlProject/MeshData.h)

target_link_libraries(GlProject
        ${OPENGL_gl_LIBRARY}
        glfw
        libGLEW.so)