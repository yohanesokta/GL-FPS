
#ifndef GLOBALS_H
#define GLOBALS_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "wrapper/font.hpp"
#include "objects/enemy.hpp"
#include <vector>

// Macros 
#define ESC_KEY 27
#define R_KEY 114
#define MAX_AMMO 10
#define BYTE 1024


namespace Enemy {
    class enemy; 
}

typedef struct EnemyColosion {
    int id;
    float x, y, z;
    bool isColliding;
} EnemyColosion;

typedef struct BasicColosion {
    float x1, y1, z1, x2, y2, z2;
} BasicColosion;

typedef struct Bullet {
    float x, y, z;
    float dirX, dirY, dirZ;
    int step;
    bool isActive;
} Bullet;


typedef struct EnemyIsColliding {
    int id;
    bool isColliding;
} EnemyIsColliding;

extern bool enemyId[4*BYTE];
extern Enemy::enemy enemy1;
extern Enemy::enemy enemy2;
extern std::vector<Bullet> bullets;
extern EnemyColosion enemyColosionList[4*BYTE];
extern std::vector<BasicColosion> basicColosionList;
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
extern GLuint FloorTexture, WallTexture, AmmoTexture, CellingTexture, BesiTexture,
       Barrel1Texture,PullPropsTexture, NodPropsTexture, BulletTexture
;
extern float get_yaw_to_player(float ox, float oz, float px, float pz);
extern GLuint GunSprite[40];
extern float isMusicPlaying;
extern char bassePath[256];
extern char* getAssets( const char* filename);
extern bool checkAllCollisionsBasic(float x, float y, float z);
extern EnemyIsColliding checkEnemyCollisions(float x, float y, float z);
// Game state
extern bool isShooting;
extern bool isReloading;
extern int bullet;
extern int magazine;
extern int image_index;
extern float shotTime;
extern bool keys[256];
extern bool special[256];
extern float lastTime;

// Font
extern Font globalFont;


#endif
