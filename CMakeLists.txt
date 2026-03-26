cmake_minimum_required(VERSION 3.10)

project(GLUTApp VERSION 1.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(OpenGL_GL_PREFERENCE GLVND)

find_package(OpenGL REQUIRED)

add_executable(main
    src/main.cpp
    src/globals.cpp
    src/loader.cpp
    src/player.cpp
    src/renderer.cpp
    libs/d3d/d3d.cpp
)

target_include_directories(main PRIVATE
    src
    libs/d3d
)

if (UNIX AND NOT WIN32)
    message(STATUS "Building for Linux (system GLUT)")

    find_package(GLUT REQUIRED)

    target_link_libraries(main PRIVATE
        OpenGL::GL
        OpenGL::GLU
        GLUT::GLUT
    )
endif()

if (WIN32)
    message(STATUS "Building for Windows (local freeglut)")

    target_include_directories(main PRIVATE
        ${CMAKE_SOURCE_DIR}/libs/freeglut/include
    )

    target_link_directories(main PRIVATE
        ${CMAKE_SOURCE_DIR}/libs/freeglut/lib/x64
    )

    target_link_libraries(main PRIVATE
        OpenGL::GL
        OpenGL::GLU
        freeglut
    )

    add_custom_command(TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/libs/freeglut/bin/x64/freeglut.dll
        $<TARGET_FILE_DIR:main>
    )
endif()

install(TARGETS main DESTINATION bin)