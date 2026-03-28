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
#include "wrapper/audio.hpp"



void createColosion() {
    basicColosionList.push_back({-10, 0, -10, -5, 5, -5});
    basicColosionList.push_back({0, 0, 0, 20, 10, 0});
    basicColosionList.push_back({20, 0, 0, 20, 10, 20});
    basicColosionList.push_back({10, 0, -10, 15, 10, -5});
    basicColosionList.push_back({-15, 0, 10, -5, 10, 0});
}


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


void keySpecialDown(int key, int x, int y) {
    special[key] = true;
    printf("Special key pressed: %d\n", key);
}

void keySpecialUp(int key, int x, int y) {
    special[key] = false;
    printf("Special key released: %d\n", key);
}

void bulletUpdate() {
    for (auto& bullet : bullets) {
        if (bullet.isActive) {
            bullet.step ++;
            bullet.x += bullet.dirX * 0.1f ;
            bullet.z += bullet.dirZ * 0.1f;
            if (checkAllCollisionsBasic(bullet.x, bullet.y, bullet.z)) {
                bullet.isActive = false;
            }
            EnemyIsColliding collisionResult = checkEnemyCollisions(bullet.x, bullet.y, bullet.z);
            if (collisionResult.isColliding) {
                printf("Bullet hit enemy with ID: %d\n", collisionResult.id);
                bullet.isActive = false;
                enemyId[collisionResult.id] = true;
            }
        }
    }
}

void idle() {
    updatePlayer();
    bulletUpdate();
    glutPostRedisplay();
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    stbi_set_flip_vertically_on_load(true);
    for (int i = 0; i < 40; i++) {
        char filename[256];
        sprintf(filename, "/pistol/out_%d.png", i);
        GunSprite[i] = loadTexture(getAssets(filename)); 
    }

    createColosion();
    FloorTexture = loadTexture(getAssets("/floor.png"));
    WallTexture = loadTexture(getAssets("/wall2.png"));
    BesiTexture = loadTexture(getAssets("/besi.jpg"));
    AmmoTexture = loadTexture(getAssets("/hud/ammo.png"));
    CellingTexture = loadTexture(getAssets("/celling.png"));
    Barrel1Texture = loadTexture(getAssets("/props/barrel-1.png"));
    NodPropsTexture = loadTexture(getAssets("/props/nod.png"));
    PullPropsTexture = loadTexture(getAssets("/props/pull.png"));
    enemy1.texture = loadTexture(getAssets("/enemy/enemy-1.png"));
    enemy2.texture = loadTexture(getAssets("/enemy/enemy-1.png"));
    BulletTexture = loadTexture(getAssets("/props/bullets.png"));
    HUD_HEALTH_Texture = loadTexture(getAssets("/hud/health.png"));
    enemy1.generateColosion();
    enemy2.generateColosion();
    stbi_set_flip_vertically_on_load(false);

    if (!loadFont(globalFont, getAssets("/fonts/retrogaming.ttf"), 32)) {
        
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
    glutSetCursor(GLUT_CURSOR_NONE);
}

void controlView(int mouse_x, int mouse_y) {
    angle += (mouse_x - (windowW/2.0f))/500.0f;
    glutWarpPointer(windowW / 2, windowH / 2);
}

int main(int argc, char** argv) {
    
    if (argc > 1) {
        strncpy(bassePath, argv[1], sizeof(bassePath) - 1);
        bassePath[sizeof(bassePath) - 1] = '\0';
    }
    printf("Using asset base path: %s\n", bassePath);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Doom Ku Dewe");
    init();
    glutDisplayFunc(renderScene);

    if (!Audio::Manager::init()) {
        fprintf(stderr, "Failed to initialize audio\n");
        return 1;
    }

    glutPassiveMotionFunc(controlView);
    glutReshapeFunc(reshape);
    glutKeyboardUpFunc(keyUp);
    glutKeyboardFunc(keyDown);
    glutSpecialFunc(keySpecialDown);
    glutSpecialUpFunc(keySpecialUp);
    glutMouseFunc(handleMouse);
    
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
