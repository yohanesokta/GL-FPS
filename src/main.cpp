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
#include "utils/map_manager.hpp"
#include "../libs/stb_image.h"
#include "wrapper/audio.hpp"
#include "string"
#include "utils/notify.h"

extern MapSystem::Map* g_map1;

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

void loadAssets() {
    

    if (loadingIndex < 40) {
        char filename[256];
        sprintf(filename, "/pistol/out_%d.png", loadingIndex);
        GunSprite[loadingIndex] = loadTexture(getAssets(filename));
        loadingIndex++;
    } 
    else if (loadingIndex == 40) { FloorTexture = loadTexture(getAssets("/floor.png")); loadingIndex++; }
    else if (loadingIndex == 41) { WallTexture = loadTexture(getAssets("/wall2.png")); loadingIndex++; }
    else if (loadingIndex == 42) { BesiTexture = loadTexture(getAssets("/besi.jpg")); loadingIndex++; }
    else if (loadingIndex == 43) { AmmoTexture = loadTexture(getAssets("/hud/ammo.png")); loadingIndex++; }
    else if (loadingIndex == 44) { CellingTexture = loadTexture(getAssets("/celling.png")); loadingIndex++; }
    else if (loadingIndex == 45) { Barrel1Texture = loadTexture(getAssets("/props/barrel-1.png")); loadingIndex++; }
    else if (loadingIndex == 46) { NodPropsTexture = loadTexture(getAssets("/props/nod.png")); loadingIndex++; }
    else if (loadingIndex == 47) { PullPropsTexture = loadTexture(getAssets("/props/pull.png")); loadingIndex++; }
    else if (loadingIndex == 48) { enemy1.texture = loadTexture(getAssets("/enemy/enemy-1.png")); loadingIndex++; }
    else if (loadingIndex == 49) { enemy2.texture = loadTexture(getAssets("/enemy/enemy-1.png")); loadingIndex++; }
    else if (loadingIndex == 50) { BulletTexture = loadTexture(getAssets("/props/bullets.png")); loadingIndex++; }
    else if (loadingIndex == 51) { HUD_HEALTH_Texture = loadTexture(getAssets("/hud/health.png")); loadingIndex++; }
    else if (loadingIndex == 52) { mag1.texture = loadTexture(getAssets("/props/bullet-drop.png")); loadingIndex++; }
    else if (loadingIndex == 53) { 
        MarkerTexture = loadTexture(getAssets("/props/marker.png"));
        med1.texture = loadTexture(getAssets("/props/medkit.png"));
        textureMap["besi"] = loadTexture(getAssets("/besi.jpg"));
        textureMap["wall"] = loadTexture(getAssets("/wall2.png"));
        textureMap["door-open1"] = loadTexture(getAssets("/texture/door-open1.png"));
        textureMap["cement"] = loadTexture(getAssets("/texture/cement.png"));
        FloorTexture2 = loadTexture(getAssets("/texture/floor2.png"));
        loadingIndex++;
    }
    else if (loadingIndex == 54) {
        MapSystem::load(g_map1);
        loadingIndex++;
    }
    
    loadingProgress = (float)loadingIndex / 55.0f * 100.0f;
    
    if (loadingIndex >= 55) {
        
        currentState = STATE_PLAYING;
        Audio::Manager::playMusic(getAssets("/sound/ambient.ogg"), true);
        glutSetCursor(GLUT_CURSOR_NONE);
        stbi_set_flip_vertically_on_load(false);
    }
}

int chanelStepSound = -1;

void keyDown(unsigned char key, int x, int y) {
    if (currentState == STATE_MENU) {
        if (key == 13) {
            currentState = STATE_PLAYING;
            glutSetCursor(GLUT_CURSOR_NONE);
        }
        if (key == 27) { 
            exit(0);
        }
    } else if (currentState == STATE_PLAYING) {
        keys[key] = true;
        if (key == 27) {
            currentState = STATE_MENU;
            glutSetCursor(GLUT_CURSOR_INHERIT);
        }
    }
    if ( key == 'w' || key == 'a' || key == 's' || key == 'd' ) {
        if (chanelStepSound == -1) {
            chanelStepSound = Audio::Manager::playSound(getAssets("/sound/pl_step.wav"), -1);
        }
    }
}

void keyUp(unsigned char key, int x, int y) {
    keys[key] = false;
    if ( key == 'w' || key == 'a' || key == 's' || key == 'd' ) {
        if (chanelStepSound != -1) {
            Audio::Manager::stopChannel(chanelStepSound);
            chanelStepSound = -1;
        }
    }
}

void keySpecialDown(int key, int x, int y) {
    special[key] = true;
}

void keySpecialUp(int key, int x, int y) {
    special[key] = false;
}

void idle() {
    if (currentState == STATE_LOADING) {
        loadAssets();
    } else if (currentState == STATE_PLAYING) {
        updatePlayer();
        MapSystem::update(0.016f);
    }
    glutPostRedisplay();
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    stbi_set_flip_vertically_on_load(true);

    if (!loadFont(globalFont, getAssets("/fonts/retrogaming.ttf"), 32)) {
        char msg_err[256];
        sprintf(msg_err, "Assets not found in %s. use parameter <path> to customize the asset path", getAssets("/"));
        message_handler("Gabisa Load Assets Mas", msg_err, "-e");
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

void controlView(int mouse_x, int mouse_y) {
    if (currentState == STATE_PLAYING) {
        angle += (mouse_x - (windowW/2.0f))/500.0f;
        glutWarpPointer(windowW / 2, windowH / 2);
    }
}


void systemHUD() {
    printf("\n");
    printf("========================================\n");
    printf("            [ CONTROL GUIDE ]           \n");
    printf("========================================\n");
    printf("  W / A / S / D    : Move (Forward / Left / Back / Right)\n");
    printf("  Arrow Left/Right : Rotate View Angle\n");
    printf("  Mouse Left       : Shoot\n");
    printf("  Space            : Shoot\n");
    printf("========================================\n");
    printf("\n");
}

int main(int argc, char** argv) {
    if (argc > 1) {
        strncpy(bassePath, argv[1], sizeof(bassePath) - 1);
        bassePath[sizeof(bassePath) - 1] = '\0';
    }
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Doom Ku Dewe");
    
    init();
    
    if (!Audio::Manager::init()) {
        fprintf(stderr, "Failed to initialize audio\n");
    }
    systemHUD();

    glutDisplayFunc(renderScene);
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
