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
    src/wrapper/d3d.cpp
    src/wrapper/audio.cpp
    src/objects/enemy.cpp
)

target_include_directories(main PRIVATE
    src
    src/wrapper
)

# -------------------------
# Platform-specific
# -------------------------
if (UNIX AND NOT WIN32)
    message(STATUS "Building for Linux (system GLUT/SDL2)")

    find_package(GLUT REQUIRED)

    find_package(SDL2 REQUIRED)
    find_package(SDL2_mixer REQUIRED)

    target_include_directories(main PRIVATE
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_MIXER_INCLUDE_DIRS}
    )

    target_link_libraries(main PRIVATE
        OpenGL::GL
        OpenGL::GLU
        GLUT::GLUT
        ${SDL2_LIBRARIES}
        ${SDL2_MIXER_LIBRARIES}
    )
endif()
if (WIN32)
    target_include_directories(main PRIVATE
        ${CMAKE_SOURCE_DIR}/libs/freeglut/include
        ${CMAKE_SOURCE_DIR}/libs/SDL2/include
        ${CMAKE_SOURCE_DIR}/libs/SDL2_mixer/include
    )

    target_link_directories(main PRIVATE
        ${CMAKE_SOURCE_DIR}/libs/freeglut/lib/x64
        ${CMAKE_SOURCE_DIR}/libs/SDL2/lib/x64
        ${CMAKE_SOURCE_DIR}/libs/SDL2_mixer/lib/x64
    )

    target_link_libraries(main PRIVATE
        OpenGL::GL
        OpenGL::GLU
        freeglut
        SDL2
        SDL2main
        SDL2_mixer
    )

    # Copy DLLs ke folder exe
    add_custom_command(TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/libs/freeglut/bin/x64/freeglut.dll
        $<TARGET_FILE_DIR:main>
    )
    add_custom_command(TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/libs/SDL2/lib/x64/SDL2.dll
        $<TARGET_FILE_DIR:main>
    )
    add_custom_command(TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/libs/SDL2_mixer/lib/x64/SDL2_mixer.dll
        $<TARGET_FILE_DIR:main>
    )
endif()

install(TARGETS main DESTINATION bin)