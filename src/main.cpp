#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include "globals.h"
#include "loader.h"
#include "player.h"
#include "renderer.h"
#include "../libs/stb_image.h"

void reshape(int w, int h) {
    windowW = w;
    windowH = h;

    float currentRatio = (float)w / (float)h;
    int viewW, viewH, viewX, viewY;

    if (currentRatio > targetRatio) {
        viewH = h;
        viewW = h * targetRatio;
        viewX = (w - viewW) / 2;
        viewY = 0;
    } else {
        viewW = w;
        viewH = w / targetRatio;
        viewX = 0;
        viewY = (h - viewH) / 2;
    }

    glViewport(viewX, viewY, viewW, viewH);
}

void keyDown(unsigned char key, int x, int y) {
    keys[key] = true;
    printf("Key pressed: %d\n", key);
}

void keyUp(unsigned char key, int x, int y) {
    keys[key] = false;
    printf("Key released: %d\n", key);
}

void idle() {
    updatePlayer();
    glutPostRedisplay();
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    stbi_set_flip_vertically_on_load(true);
    for (int i = 1; i <= 16; i++) {
        char filename[256];
        sprintf(filename, "../assets/gun/sprite-%d.png", i);
        GunSprite[i-1] = loadTexture(filename);
    }

    FloorTexture = loadTexture("../assets/floor.png");
    WallTexture = loadTexture("../assets/wall.png");
    stbi_set_flip_vertically_on_load(false);

    if (!loadFont(globalFont, "../assets/fonts/retrogaming.ttf", 32)) {
        fprintf(stderr, "Failed to load font\n");
        exit(1);
    }

    glEnable(GL_FOG);
    GLfloat fogColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_START,  15.0f);
    glFogf(GL_FOG_END,  50.0f);
    glHint(GL_FOG_HINT, GL_NICEST);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Doom Ku Dewe");

    init();

    glutDisplayFunc(renderScene);
    
    glutReshapeFunc(reshape);
    glutKeyboardUpFunc(keyUp);
    glutKeyboardFunc(keyDown);
    glutMouseFunc(handleMouse);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
