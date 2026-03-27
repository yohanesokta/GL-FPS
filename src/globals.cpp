#include "globals.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int windowW = 800;
int windowH = 600;
const float targetRatio = 800.0f / 600.0f;

float isMusicPlaying = false;
float camX = 0, camY = 2, camZ = 5;
float angle = 90.0f;
float lx = sin(90.0f), lz = -cos(90.0f);
float speed = 5.0;

Enemy::enemy enemy1;
Enemy::enemy enemy2;


GLuint FloorTexture, WallTexture, AmmoTexture, CellingTexture, BesiTexture,
       Barrel1Texture, PullPropsTexture, NodPropsTexture
;

char bassePath[256] = "./assets";
char* getAssets(const char* filename) {
    static char path[256];
    sprintf(path, "%s%s", bassePath, filename);
    return path;
}

GLuint GunSprite[40];
bool isShooting = false;
bool isReloading = false;
int bullet = 10;
int magazine = 10;
int image_index = 0;
float shotTime = 0;
bool keys[256];
float lastTime = 0;

typedef struct EnemyColosion {
    int id;
    float x1, y1, z1, x2, y2, z2;
} EnemyColosion;


EnemyColosion enemyColosion[] = {{1, -0.75f, 0.0f, -0.75f, 0.75f, 3.0f, 0.75f},
                                {2, -0.75f, 0.0f, -0.75f, 0.75f, 3.0f, 0.75f}};



Font globalFont;


float get_yaw_to_player(float ox, float oz, float px, float pz) {
    float dx = px - ox;
    float dz = pz - oz;
    
    float angleRad = atan2(dx, dz); 
    float angleDeg = angleRad * 180.0f / M_PI ;
    
    return angleDeg;
}
