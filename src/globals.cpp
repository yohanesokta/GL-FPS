#include "globals.h"
#include "objects/magazine.hpp"
#include "objects/medkit.hpp"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int windowW = 800;
int windowH = 600;

bool isCustomMap = false;
std::string customMapPath = "";


const float targetRatio = 800.0f / 600.0f;

std::string comments = "hello world";
float isMusicPlaying = false;
float camX = 46.0f, camY = 2.0f, camZ = 94.0f;

float angle = 89.5f;
float lx = sin(angle), lz = -cos(angle);
float speed = 5.0;
float deltaTime = 0.0f;
float health = 5.0f;

float loadingProgress = 0.0f;
int loadingIndex = 0;

Enemy::enemy enemy1(2.0f, 2.0f, 0);
Enemy::enemy enemy2(10.0f, 10.0f, 1);
Magazine::magazine mag1(10.0f, 10.0f, 5);
Medkit::medkit med1(10.0f, 20.0f, 25.0f);

std::unordered_map<std::string, GLuint> textureMap;

GLuint FloorTexture, WallTexture, AmmoTexture, CellingTexture, BesiTexture, EnemyTexture,
       Barrel1Texture, PullPropsTexture, NodPropsTexture, BulletTexture, HUD_HEALTH_Texture, FloorTexture2, MarkerTexture,
       HUD_Senter;

char bassePath[256] = "./assets";
char* getAssets(const char* filename) {
    static char path[256];
    sprintf(path, "%s%s", bassePath, filename);
    return path;
}

GLuint GunSprite[40];
bool isShooting = false;
bool isReloading = false;
int bullet = 0;
int magazine = 0;
int image_index = 0;
float shotTime = 0;
bool keys[256];
bool special[256];
float lastTime = 0;
bool enemyId[4*BYTE];
GameState currentState = STATE_LOADING;

EnemyColosion enemyColosionList[4*BYTE];
std::vector<BasicColosion> basicColosionList;
std::vector<Bullet> bullets;
Font globalFont;

float fadeAlpha = 1.0f;
bool isFadingIn = true;

float get_yaw_to_player(float ox, float oz, float px, float pz) {
    float dx = px - ox;
    float dz = pz - oz;
    
    float angleRad = atan2(dx, dz); 
    float angleDeg = angleRad * 180.0f / M_PI ;
    
    return angleDeg;
}

bool checkAllCollisionsBasic(float x, float y, float z, bool isPlayer) {
    float pr = 0.5f; 
    for (const auto& col : basicColosionList) {
        if (d3d_collision_block(x, y, z, pr, col.x1, col.y1, col.z1, col.x2, col.y2, col.z2)) {
            if (isPlayer) {
                comments = col.comments;
            }
            return true;
        }
    }
    return false;
}

EnemyIsColliding checkEnemyCollisions(float x, float y, float z) {
    float pr = 0.5f; 
    for (auto& col : enemyColosionList) {
        if (d3d_collision_ellipsoid(x, y, z, pr, col.x,1.5f * 3.0f, col.y + 1.5f * 1.5f, col.x + 1.5f * 3.0f,col.z, col.y - 1.5f * 1.5f)) {
            col.isColliding = true;
            return {col.id, true};
        }
    }
    return {-1, false};
}
