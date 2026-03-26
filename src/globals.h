#ifndef GLOBALS_H
#define GLOBALS_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "../libs/d3d/font.hpp"

// Window settings
extern int windowW;
extern int windowH;
extern const float targetRatio;

// Camera/Player position and orientation
extern float camX, camY, camZ;
extern float angle;
extern float lx, lz;
extern float speed;

// Textures
extern GLuint FloorTexture, WallTexture;
extern GLuint GunSprite[16];

// Game state
extern bool isShooting;
extern bool isReloading;
extern int bullet;
extern int magazine;
extern int image_index;
extern float shotTime;
extern bool keys[256];
extern float lastTime;

// Font
extern Font globalFont;

#define ESC_KEY 27
#define R_KEY 114

#endif
