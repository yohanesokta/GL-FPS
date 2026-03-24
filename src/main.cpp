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
float camX = 0, camY = 2, camZ = 5;
float angle = 0.0;
float lx = 0.0, lz = -1.0;
float speed = 1.0;
GLuint HandTexture;

void drawFloor() {
    glColor3f(0.5, 0.5, 0.5);

    glBegin(GL_LINES);
    for (int i = -20; i <= 20; i++) {
        glVertex3f(i, 0, -20);
        glVertex3f(i, 0, 20);

        glVertex3f(-20, 0, i);
        glVertex3f(20, 0, i);
    }
    glEnd();
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 3D WORLD
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
    // HUD
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);

    // drawing HUD
    drawTexturedQuad(HandTexture, 650, 50, 100, 100);
    
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
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Camera + Floor");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}