#ifndef LOADER_H
#define LOADER_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>

GLuint loadTexture(const char* filename);
void drawTexturedQuad(GLuint textureID, float x, float y, float width, float height);
void draw3DQuad(GLuint textureID, float x, float y, float z, float size, float rotation[3], float rotationAngle);

extern float X_ROTATE[3];
extern float Y_ROTATE[3];
extern float Z_ROTATE[3];

#endif
