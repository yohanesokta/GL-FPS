#include "renderer.h"
#include "loader.h"
#include "../libs/d3d/d3d.h"
#include <stdio.h>

void drawWorld() {
    d3d_draw_floor(-100, 0, -100, 100, 0, 100, FloorTexture, 20, 20);
    d3d_draw_floor(-100, 10, -100, 100, 10, 100, CellingTexture, 20, 20);

    d3d_draw_wall(0, 0, 0, 20, 10, 0, WallTexture, 2, 1);
    d3d_draw_wall(20, 0, 0, 20, 10, 20, WallTexture, 2, 1);

    
    d3d_draw_block(-10, 0, -10, -5, 5, -5, WallTexture, 1, 1);
    d3d_draw_cylinder(10, 0, -10, 15, 10, -5, FloorTexture, 1, 1, true, 24);
    d3d_draw_ellipsoid(-15, 0, 10, -5, 10, 0, BesiTexture, 1, 1, 24);
    d3d_draw_wall(-5,3.3f/1.2f, 0, -5,0, 3.3f/1.2f, Barrel1Texture, 1, 1);
}

void drawHUD() {
    // 2D HUD Rendering
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600); 

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    drawTexturedQuad(GunSprite[image_index], -20, 0, 720*1.3f, 480*1.3f);
    drawTexturedQuad(AmmoTexture, 2.0f, 2.0f, 438/2.2f , 264/2.2f);
    glRectf(windowW/2-0.8f, windowH/2-5, windowW/2+0.8f, windowH/2+5);
    glRectf(windowW/2-5, windowH/2-0.8f, windowW/2+5, windowH/2+0.8f);

    char magazine_text[256], bullet_text[256];
    sprintf(magazine_text, "%d", magazine);
    sprintf(bullet_text, "%d", bullet);
    renderText(globalFont, 135, windowH - 70, magazine_text, windowH);
    renderText(globalFont, 50, windowH - 70, bullet_text, windowH);
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
