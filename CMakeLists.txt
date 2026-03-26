cmake_minimum_required(VERSION 3.10)

project(GLUTApp VERSION 1.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(OpenGL_GL_PREFERENCE GLVND)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

add_executable(main
    src/main.cpp
    src/globals.cpp
    src/loader.cpp
    src/player.cpp
    src/renderer.cpp
    libs/d3d/d3d.cpp
)

target_include_directories(main PRIVATE src libs/d3d)

target_link_libraries(main PRIVATE
    OpenGL::GL
    OpenGL::GLU
    GLUT::GLUT
)

install(TARGETS main DESTINATION bin)
