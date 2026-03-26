#include "renderer.h"
#include "loader.h"
#include "../libs/d3d/d3d.h"
#include <stdio.h>

void drawWorld() {
    d3d_draw_floor(-100, 0, -100, 100, 0, 100, FloorTexture, 20, 20);
    d3d_draw_wall(0, 0, 0, 20, 10, 0, WallTexture, 1, 1);
    d3d_draw_wall(20, 0, 0, 20, 10, 20, WallTexture, 1, 1);
    d3d_draw_block(-10, 0, -10, -5, 5, -5, WallTexture, 1, 1);
    d3d_draw_cylinder(10, 0, -10, 15, 10, -5, WallTexture, 1, 1, true, 24);
    d3d_draw_ellipsoid(-15, 5, 10, -5, 15, 20, FloorTexture, 1, 1, 24);
}

void drawHUD() {
    // 2D HUD Rendering
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600); 

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    drawTexturedQuad(GunSprite[image_index], windowW/2, -10, 200, 250);
   
    glRectf(windowW/2-0.8f, windowH/2-5, windowW/2+0.8f, windowH/2+5);
    glRectf(windowW/2-5, windowH/2-0.8f, windowW/2+5, windowH/2+0.8f);

    char bulletText[256];
    if (isReloading) {
        sprintf(bulletText, "Reloading... Magazine: %d", magazine);
    } else {
        sprintf(bulletText, "Bullets: %d  Magazine: %d", bullet, magazine);
    }
    
    glColor3f(1.0f, 1.0f, 1.0f);
    renderText(globalFont, 10, windowH/2, bulletText, windowH);
}

void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, targetRatio, 0.1, 100);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        camX, camY, camZ,
        camX + lx, camY, camZ + lz,
        0.0, 1.0, 0.0
    );

    glEnable(GL_DEPTH_TEST);
    drawWorld();
    drawHUD();

    glutSwapBuffers();
}
