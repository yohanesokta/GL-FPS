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

float camX = 0, camY = 2, camZ = 5;
float angle = 90.0f;
float lx = sin(angle), lz = -cos(angle);
float speed = 5.0;
GLuint HandTexture, FloorTexture, WallTexture;

void drawFloor() {
    d3d_draw_floor(-100, 0, -100, 100, 0, 100, FloorTexture, 10, 10);
    d3d_draw_wall(0, 0, 0, 20, 10, 0, WallTexture, 1, 1);
    d3d_draw_wall(20, 0, 0, 20, 10, 20, WallTexture, 1, 1);
    d3d_draw_block(-10, 0, -10, -5, 5, -5, WallTexture, 1, 1);
    d3d_draw_cylinder(10, 0, -10, 15, 10, -5, WallTexture, 1, 1, true, 24);
    d3d_draw_ellipsoid(-15, 5, 10, -5, 15, 20, FloorTexture, 1, 1, 24);
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 800.0/600.0, 0.1, 100);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(
        camX, camY, camZ,
        camX + lx, camY, camZ + lz,
        0.0, 1.0, 0.0
    );

    glEnable(GL_DEPTH_TEST);
    drawFloor();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);

    drawTexturedQuad(HandTexture, 800/2, 0, 350, 350);
    
    glutSwapBuffers();
}
void reshape(int w, int h) {
    if (h == 0) h = 1;
    float ratio = 1.0 * w / h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45, ratio, 0.1, 100);

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

    
}



bool keys[256];

void keyDown(unsigned char key, int x, int y) {
    keys[key] = true;
}
void keyUp(unsigned char key, int x, int y) {
    keys[key] = false;
}


float lastTime = 0;

void update() {
    float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    float deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    float moveSpeed = 5.0f * deltaTime * speed;
    float rotSpeed  = 2.0f * deltaTime;

    if (keys['w']) {
        camX += lx * moveSpeed;
        camZ += lz * moveSpeed;
    }
    if (keys['s']) {
        camX -= lx * moveSpeed;
        camZ -= lz * moveSpeed;
    }
    if (keys['a']) angle -= rotSpeed;
    if (keys['d']) angle += rotSpeed;

    lx = sin(angle);
    lz = -cos(angle);
}

void idle() {
    update();
    glutPostRedisplay();
}
void keyboard(unsigned char key, int x, int y) {
    if (key == 'w') {
        camX += lx * speed;
        camZ += lz * speed;
    }
    if (key == 's') {
        camX -= lx * speed;
        camZ -= lz * speed;
    }
    if (key == 'a') {
        angle -= 0.05;
        lx = sin(angle);
        lz = -cos(angle);
    }
    if (key == 'd') {
        angle += 0.05;
        lx = sin(angle);
        lz = -cos(angle);
    }

    glutPostRedisplay();
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    stbi_set_flip_vertically_on_load(true);
    HandTexture = loadTexture("../assets/hand/Pistolidle.png");
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
    glutIdleFunc(idle);
    glutMainLoop();


    return 0;
}