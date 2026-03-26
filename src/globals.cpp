#include "globals.h"
#include <cmath>

int windowW = 800;
int windowH = 600;
const float targetRatio = 800.0f / 600.0f;

float camX = 0, camY = 2, camZ = 5;
float angle = 90.0f;
float lx = sin(90.0f), lz = -cos(90.0f);
float speed = 5.0;

GLuint FloorTexture, WallTexture, AmmoTexture, CellingTexture, BesiTexture;
GLuint GunSprite[16];

bool isShooting = false;
bool isReloading = false;
int bullet = 2;
int magazine = 10;
int image_index = 1;
float shotTime = 0;
bool keys[256];
float lastTime = 0;

Font globalFont;
