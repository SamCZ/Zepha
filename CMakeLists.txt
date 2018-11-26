cmake_minimum_required(VERSION 3.12)
project(GlProject)

set(CMAKE_CXX_STANDARD 14)

include_directories(GlProject/Libraries/glew/include/GL)
include_directories(GlProject/Libraries/glfw_linux/include)
include_directories(GlProject/Libraries/glm)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

link_directories(GlProject/Libraries/glew/lib)

add_executable(GlProject
        GlProject/Main.cpp GlProject/Mesh.cpp GlProject/Mesh.h GlProject/Entity.cpp GlProject/Entity.h)

target_link_libraries(GlProject
        ${OPENGL_gl_LIBRARY}
        glfw
        libGLEW.so)