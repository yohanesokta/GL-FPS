#include "renderer.h"
#include "loader.h"
#include "wrapper/d3d.h"
#include "utils/map_manager.hpp"
#include <stdio.h>

void drawWorld() {
    MapSystem::render();
}

void drawHUD() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600); 

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    if (!(magazine == 0 && bullet == 0)) {
        drawTexturedQuad(GunSprite[image_index], -20, 0, 720*1.3f, 480*1.3f);
    }
    drawTexturedQuad(AmmoTexture, 2.0f, 2.0f, 438/2.2f , 264/2.2f);
    drawTexturedQuad(HUD_HEALTH_Texture, 210.0f, 2.0f, 191/2.2f, 262/2.2f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glRectf(258.0f, 17.0f,260.0f + 21.0f , 17.0f + (87.0f/100.0f)*health);
    glColor3f(1.0f, 1.0f, 1.0f);
    glRectf(windowW/2-0.8f, windowH/2-5, windowW/2+0.8f, windowH/2+5);
    glRectf(windowW/2-5, windowH/2-0.8f, windowW/2+5, windowH/2+0.8f);

    char magazine_text[256], bullet_text[256];
    sprintf(magazine_text, "%d", magazine);
    sprintf(bullet_text, "%d", bullet);
    renderText(globalFont, 135, windowH - 70, magazine_text, windowH);
    renderText(globalFont, 50, windowH - 70, bullet_text, windowH);
    
    char xplayer[256], yplayer[256];
    sprintf(xplayer, "X: %.2f", camX);
    sprintf(yplayer, "Y: %.2f", camZ);
    char comment[256];
    sprintf(comment, "%s", ::comments.c_str());
    renderText(globalFont, 10, windowH - 260, comment, windowH+1.5f);
    renderText(globalFont, 10, windowH - 230, xplayer, windowH+1.5f);
    renderText(globalFont, 10, windowH - 200, yplayer, windowH+1.5f);

}

void drawLoading() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    renderText(globalFont, 320, 320, "LOADING...", 600);

    float barWidth = 400.0f;
    float barHeight = 20.0f;
    float startX = (800.0f - barWidth) / 2.0f;
    float startY = 250.0f;

    glBegin(GL_LINE_LOOP);
    glVertex2f(startX, startY);
    glVertex2f(startX + barWidth, startY);
    glVertex2f(startX + barWidth, startY + barHeight);
    glVertex2f(startX, startY + barHeight);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(startX, startY);
    glVertex2f(startX + (barWidth * (loadingProgress / 100.0f)), startY);
    glVertex2f(startX + (barWidth * (loadingProgress / 100.0f)), startY + barHeight);
    glVertex2f(startX, startY + barHeight);
    glEnd();

    char progressText[32];
    sprintf(progressText, "%.0f%%", loadingProgress);
    renderText(globalFont, 380, 220, progressText, 600);
}
void drawMenu() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 600, 0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBegin(GL_QUADS);
        glColor3f(0.03f, 0.03f, 0.08f);
        glVertex2f(0, 0);
        glVertex2f(800, 0);

        glColor3f(0.08f, 0.0f, 0.15f);
        glVertex2f(800, 600);
        glVertex2f(0, 600);
    glEnd();
    
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    for (int i = 0; i < 800; i += 50) {
        glColor3f(0.15f, 0.15f, 0.25f);
        glVertex2f(i, 0);
        glVertex2f(i, 600);
    }
    for (int i = 0; i < 600; i += 50) {
        glColor3f(0.15f, 0.15f, 0.25f);
        glVertex2f(0, i);
        glVertex2f(800, i);
    }
    glEnd();

    glColor3f(0.6f, 0.8f, 1.0f);
    renderTextCentered(globalFont, 400, 200, "FPS PROTOTYPE", 1.6f);
    glColor3f(0.1f, 0.1f, 0.1f);
    renderTextCentered(globalFont, 402, 202, "FPS PROTOTYPE", 1.6f);
    glColor3f(1.0f, 0.4f, 0.4f);
    renderTextCentered(globalFont, 400, 240, "belajar menghargai diri sendiri, sejak kamu memilih menghancurkannya.", 0.5f);
    glColor3f(1.0f, 1.0f, 1.0f);
    renderTextCentered(globalFont, 400, 260, "Jangan bungkus pengkhianatanmu dengan kata “maaf” seolah semuanya jadi selesai.", 0.5f);

    glColor3f(1.0f, 1.0f, 1.0f);
    renderTextCentered(globalFont, 400, 350, "PRESS ENTER TO START", 0.8f);
    renderTextCentered(globalFont, 400, 380, "PRESS ESC TO QUIT", 0.6f);
    glColor3f(1.0f, 1.0f, 1.0f);
}
void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (currentState == STATE_LOADING) {
        drawLoading();
    } else if (currentState == STATE_MENU) {
        drawMenu();
    } else {
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
    }

    glutSwapBuffers();
}
