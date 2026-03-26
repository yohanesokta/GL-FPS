#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <_stdlib.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>

#include "loader.cpp"
#include "d3d.h"
#include <cmath>

float camX = 0, camY = 2, camZ = 5;
float angle = 90.0f;
float lx = sin(angle), lz = -cos(angle);
float speed = 5.0;
GLuint FloorTexture, WallTexture;

void drawFloor() {
    d3d_draw_floor(-100, 0, -100, 100, 0, 100, FloorTexture, 10, 10);
    d3d_draw_wall(0, 0, 0, 20, 10, 0, WallTexture, 1, 1);
    d3d_draw_wall(20, 0, 0, 20, 10, 20, WallTexture, 1, 1);
    d3d_draw_block(-10, 0, -10, -5, 5, -5, WallTexture, 1, 1);
    d3d_draw_cylinder(10, 0, -10, 15, 10, -5, WallTexture, 1, 1, true, 24);
    d3d_draw_ellipsoid(-15, 5, 10, -5, 15, 20, FloorTexture, 1, 1, 24);
}

GLuint GunSprite[16];
bool isShooting = false;
bool isReloading = false;
int bullet = 5;
int magazine = 10;
int image_index = 0;
float shotTime = 0;

int windowW = 800;
int windowH = 600;
const float targetRatio = 800.0f / 600.0f;

void display() {
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
    drawFloor();

    // 2D HUD Rendering
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600); 

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    drawTexturedQuad(GunSprite[image_index], 800/2, -10, 150, 200);
   
    glRectf(windowW/2-0.8f,windowH/2-5,windowW/2+0.8f,windowH/2+5);
    glRectf(windowW/2-5,windowH/2-0.8f,windowW/2+5,windowH/2+0.8f);
    char bulletText[256];
    sprintf(bulletText, "Bullets: %d  Magazine: %d", bullet, magazine);
    renderText(10,10, GLUT_BITMAP_HELVETICA_18, bulletText);

    glutSwapBuffers();
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


bool keys[256];

void keyDown(unsigned char key, int x, int y) {
    keys[key] = true;
}
void keyUp(unsigned char key, int x, int y) {
    keys[key] = false;
}


float lastTime = 0;

bool checkAllCollisions(float x, float y, float z) {
    float pr = 0.5f; 
    if (d3d_collision_block(x, y, z, pr, -10, 0, -10, -5, 5, -5)) return true;
    if (d3d_collision_cylinder(x, y, z, pr, 10, 0, -10, 15, 10, -5)) return true;
    if (d3d_collision_ellipsoid(x, y, z, pr, -15, 5, 10, -5, 15, 20)) return true;
    return false;
}

void update() {
    float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    float deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    float moveSpeed = 5.0f * deltaTime * speed;
    float rotSpeed  = 2.0f * deltaTime;

    float nextX = camX;
    float nextZ = camZ;

    if (keys['w']) {
        nextX += lx * moveSpeed;
        nextZ += lz * moveSpeed;
    }
    if (keys['s']) {
        nextX -= lx * moveSpeed;
        nextZ -= lz * moveSpeed;
    }
    if (keys['a']) angle -= rotSpeed;
    if (keys['d']) angle += rotSpeed;

    if (keys[32]) {
        if (!isShooting && bullet > 0) {
            isShooting = true;
            bullet -= 1;
        }
    }

    
    if (!checkAllCollisions(nextX, camY, camZ)) {
        camX = nextX;
    }
    if (!checkAllCollisions(camX, camY, nextZ)) {
        camZ = nextZ;
    }

    lx = sin(angle);
    lz = -cos(angle);


    // shooting animation
    if (isShooting) {
        shotTime += 0.1f;
        image_index = static_cast<int>(shotTime);
        if (image_index >= 5) {
            image_index =0;
            isShooting = false;
            shotTime = 0;
        }
    }

    if (bullet == 0 && !isReloading && magazine > 0) {
        isReloading = true;
    }

    if (isReloading) {
        shotTime += 0.1f;
        image_index = 9 + static_cast<int>(shotTime);
        if (image_index >= 15) {
            image_index = 0;
            isReloading = false;
            shotTime = 0;
            bullet = 5;
            magazine -= 1;
        }
    }
}

void idle() {
    update();
    glutPostRedisplay();
}

void mouseHandle(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (!isShooting && bullet > 0) {
            isShooting = true;
            bullet -=1;
        }
    }
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    stbi_set_flip_vertically_on_load(true);
    for (int i = 1; i < 16; i++) {
        char filename[256];
        sprintf(filename, "../assets/gun/sprite-%d.png", i);
        GunSprite[i-1] = loadTexture(filename);
    }

    FloorTexture = loadTexture("../assets/floor.png");
    WallTexture = loadTexture("../assets/wall.png");
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Doom Ku Dewe");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardUpFunc(keyUp);
    glutKeyboardFunc(keyDown);
    glutMouseFunc(mouseHandle);
    glutIdleFunc(idle);
    glutMainLoop();

    return 0;
}