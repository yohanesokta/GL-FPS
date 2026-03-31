#include "../../libs/json.hpp"
#include "../utils/map_system.hpp"
#include "globals.h"
#include "objects/enemy.hpp"
#include "objects/magazine.hpp"
#include "objects/medkit.hpp"
#include "wrapper/audio.hpp"
#include "wrapper/d3d.h"
#include <algorithm>
#include <fstream>

using json = nlohmann::json;

GLuint getTextureByName(const std::string &name) {
  if (textureMap.find(name) != textureMap.end())
    return textureMap[name];

  return WallTexture;
}

struct Wall {
  float x1, y1, z1, x2, y2, z2;
  GLuint texture = WallTexture;
};

float zDoorOpen = 0.0f;
bool isDoorOpening = false;
float stepDoor = 0.0f;

std::vector<Enemy::enemy> enemyList;
std::vector<Magazine::magazine> mags;
std::vector<Medkit::medkit> meds;
std::vector<Wall> walls;

Magazine::magazine createMag(float x, float z, int ammo) {
  return Magazine::magazine(x, z, ammo);
}

Medkit::medkit createMed(float x, float z, float health) {
  return Medkit::medkit(x, z, health);
}

Enemy::enemy createEnemy(float x, float z, int id) {
  Enemy::enemy e(x, z, id);
  e.generateColosion();
  return e;
}
void loadMap(const std::string &path) {
  std::ifstream file(path);

  if (!file.is_open()) {
    printf("FAILED OPEN MAP: %s\n", path.c_str());
    return;
  }

  json data;

  try {
    file >> data;
  } catch (std::exception &e) {
    printf("JSON ERROR: %s\n", e.what());
    return;
  }

  walls.clear();
  enemyList.clear();
  mags.clear();
  meds.clear();

  if (data.contains("walls")) {
    for (auto &w : data["walls"]) {
      walls.push_back({w.value("x1", 0.0f), w.value("y1", 0.0f),
                       w.value("z1", 0.0f), w.value("x2", 0.0f),
                       w.value("y2", 0.0f), w.value("z2", 0.0f),
                       getTextureByName(w.value("texture", "wall"))});
    }
  }

  if (data.contains("enemies")) {
    for (auto &e : data["enemies"]) {
      enemyList.push_back(createEnemy(e.value("x", 0.0f), e.value("z", 0.0f),
                                      e.value("id", 0)));
    }
  }

  if (data.contains("magazines")) {
    for (auto &m : data["magazines"]) {
      mags.push_back(createMag(m.value("x", 0.0f), m.value("z", 0.0f),
                               m.value("ammo", 0)));
    }
  }

  if (data.contains("medkits")) {
    for (auto &m : data["medkits"]) {
      meds.push_back(createMed(m.value("x", 0.0f), m.value("z", 0.0f),
                               m.value("health", 0.0f)));
    }
  }

  basicColosionList.clear();
  for (auto &wall : data["walls"]) {
    basicColosionList.push_back({wall.value("x1", 0.0f), wall.value("y1", 0.0f),
                                 wall.value("z1", 0.0f), wall.value("x2", 0.0f),
                                 wall.value("y2", 0.0f), wall.value("z2", 0.0f),
                                 wall.value("comments", "")});
  }
}

class Map1 : public MapSystem::Map {
public:
  void init() override {
    basicColosionList.clear();
    bullets.clear();
    mags.clear();
    meds.clear();
    walls.clear();
    loadMap(getAssets("/maps/maps1.json"));
    basicColosionList.push_back(
        {0, 0, zDoorOpen, 11, 10, zDoorOpen + 10.0f, "Door01"});
  }

  void step(float dt) override {
    for (auto &bullet : bullets) {
      if (bullet.isActive) {
        bullet.step++;
        bullet.x += bullet.dirX * 0.5f;
        bullet.z += bullet.dirZ * 0.5f;

        if (checkAllCollisionsBasic(bullet.x, bullet.y, bullet.z)) {
          bullet.isActive = false;
        }

        EnemyIsColliding hit =
            checkEnemyCollisions(bullet.x, bullet.y, bullet.z);
        if (hit.isColliding) {
          bullet.isActive = false;
          enemyId[hit.id] = true;
        }
      }
    }

    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                                 [](const Bullet &b) { return !b.isActive; }),
                  bullets.end());
  }

  void draw() override {
    float b = 0.02f;
    d3d_draw_floor(0, 0, 0, 120, 0, 120, FloorTexture, 20, 20);
    d3d_draw_floor(0, 10, 0, 120, 10, 120, CellingTexture, 20, 20);
    d3d_draw_floor(2.5f, 0.1f, 2.5f, 10.0f, 0.1f, 8.5f, FloorTexture2, 1, 1);

    for (auto &wall : walls) {
      d3d_draw_block(wall.x1, wall.y1, wall.z1, wall.x2, wall.y2, wall.z2,
                     wall.texture, 1, 1);
    }
    for (auto &e : enemyList) {
      e.draw();
    }

    for (auto &m : mags) {
      if (!m.isPickedUp) {
        m.draw();
      }
    }

    for (auto &m : meds) {
      m.draw();
    }
    for (auto &bullet : bullets) {
      if (bullet.isActive && bullet.step > 8) {
        d3d_draw_ellipsoid(bullet.x - b, bullet.y - b, bullet.z - b,
                           bullet.x + b, bullet.y + b, bullet.z + b,
                           BulletTexture, 1, 1, 20);
      }
    }

    d3d_draw_wall_rot(15.0f + 0.6f, 1.4f, 13.0f + 0.6f, 15.0f - 0.6f, 0.0f,
                      13.0f - 0.6f, MarkerTexture, 1, 1, 45.0f);
    d3d_draw_wall_rot(18.0f + 0.6f, 1.4f, 13.0f - 0.6f, 18.0f - 0.6f, 0.0f,
                      13.0f + 0.6f, MarkerTexture, 1, 1, -45.0f);

    if (zDoorOpen < 10.0f) {
      d3d_draw_wall(11, zDoorOpen + 0.0f, 0, 11, zDoorOpen + 10.0f, 10,
                    textureMap["door-open1"], 1, 1);
    }

    if (!isDoorOpening) {
      stepDoor += deltaTime * 20.0f;
    } else {
      if (zDoorOpen < 10.0f) {
        zDoorOpen += deltaTime * 5.0f;
        if (zDoorOpen > 10.0f) {
          zDoorOpen = 10.0f;
        }
      }
    }
    if (stepDoor > 200.0f) {
      stepDoor = 0;
      isDoorOpening = true;
      Audio::Manager::playSound(getAssets("/sound/opendor.wav"));
      basicColosionList.erase(std::remove_if(basicColosionList.begin(),
                                             basicColosionList.end(),
                                             [](const BasicColosion &col) {
                                               return col.comments == "Door01";
                                             }),
                              basicColosionList.end());
    }
  }
};

Map1 map1Instance;
MapSystem::Map *g_map1 = &map1Instance;