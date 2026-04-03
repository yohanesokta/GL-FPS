#ifndef GLOBALS_H
#define GLOBALS_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "wrapper/font.hpp"
#include "objects/enemy.hpp"
#include "objects/magazine.hpp"
#include "objects/medkit.hpp"
#include <vector>
#include <string>
#include <unordered_map>

#define ESC_KEY 27
#define R_KEY 114
#define MAX_AMMO 10
#define BYTE 1024

namespace Enemy {
    class enemy; 
}
namespace Magazine {
    class magazine;
}

namespace Medkit {
    class medkit;
}

typedef struct EnemyColosion {
    int id;
    float x, y, z;
    bool isColliding;
} EnemyColosion;

typedef struct BasicColosion {
    float x1, y1, z1, x2, y2, z2;
    std::string comments;
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

enum GameState {
    STATE_LOADING,
    STATE_MENU,
    STATE_PLAYING
};

extern GameState currentState;
extern float loadingProgress;
extern int loadingIndex;
extern std::string comments;
extern bool enemyId[4*BYTE];
extern std::unordered_map<std::string, GLuint> textureMap;
extern Enemy::enemy enemy1;
extern Enemy::enemy enemy2;
extern Magazine::magazine mag1;
extern Medkit::medkit med1;
extern std::vector<Bullet> bullets;
extern EnemyColosion enemyColosionList[4*BYTE];
extern std::vector<BasicColosion> basicColosionList;

extern int windowW;
extern int windowH;
extern const float targetRatio;

extern float camX, camY, camZ;
extern float angle;
extern float lx, lz;
extern float speed;
extern float health;
extern float deltaTime;

extern GLuint FloorTexture, EnemyTexture, WallTexture, AmmoTexture, CellingTexture, BesiTexture, Barrel1Texture,PullPropsTexture, NodPropsTexture, BulletTexture, HUD_HEALTH_Texture, FloorTexture2, MarkerTexture, HUD_Senter;

extern float get_yaw_to_player(float ox, float oz, float px, float pz);
extern GLuint GunSprite[40];
extern float isMusicPlaying;
extern char bassePath[256];
extern char* getAssets( const char* filename);
extern bool checkAllCollisionsBasic(float x, float y, float z, bool isPlayer = false);
extern EnemyIsColliding checkEnemyCollisions(float x, float y, float z);

extern bool isShooting;
extern bool isReloading;
extern int bullet;
extern int magazine;
extern int image_index;
extern float shotTime;
extern bool keys[256];
extern bool special[256];
extern float lastTime;

extern Font globalFont;
extern float fadeAlpha;
extern bool isFadingIn;
#endif