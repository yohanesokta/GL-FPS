#include "../utils/map_system.hpp"
#include "globals.h"
#include "wrapper/d3d.h"
#include "objects/enemy.hpp"
#include "objects/magazine.hpp"
#include "objects/medkit.hpp"

std::vector<Enemy::enemy> enemyList;
std::vector<Magazine::magazine> mags;
std::vector<Medkit::medkit> meds;

Magazine::magazine createMag(float x, float z, int ammo)
{
    return Magazine::magazine(x, z, ammo);
}

Medkit::medkit createMed(float x, float z, float health)
{
    return Medkit::medkit(x, z, health);
}

Enemy::enemy createEnemy(float x, float z, int id)
{
    Enemy::enemy e(x, z, id);
    e.generateColosion();
    return e;
}

class Map1 : public MapSystem::Map
{
public:
    void addWall(float x1, float y1, float z1, float x2, float y2, float z2)
    {
        float step = 20.0f;

        for (float x = x1; x < x2; x += step)
        {
            for (float z = z1; z < z2; z += step)
            {
                float nx = (x + step > x2) ? x2 : x + step;
                float nz = (z + step > z2) ? z2 : z + step;

                basicColosionList.push_back({x, y1, z, nx, y2, nz});
            }
        }
    }

    void drawWall(float x1, float y1, float z1, float x2, float y2, float z2)
    {
        float step = 20.0f;

        for (float x = x1; x < x2; x += step)
        {
            for (float z = z1; z < z2; z += step)
            {
                float nx = (x + step > x2) ? x2 : x + step;
                float nz = (z + step > z2) ? z2 : z + step;

                d3d_draw_block(x, y1, z, nx, y2, nz, WallTexture, 1, 1);
            }
        }
    }

    void init() override
    {
        basicColosionList.clear();
        bullets.clear();

        mags.clear();
        meds.clear();

        // kiri area
        mags.push_back(createMag(18, 20, 5));
        mags.push_back(createMag(25, 28, 10));
        meds.push_back(createMed(20, 35, 20));

        // corridor tengah
        mags.push_back(createMag(40, 30, 8));
        mags.push_back(createMag(50, 35, 6));
        meds.push_back(createMed(45, 45, 20));

        // tikungan
        mags.push_back(createMag(60, 55, 10));
        meds.push_back(createMed(58, 50, 20));

        // kanan area
        mags.push_back(createMag(75, 70, 12));
        mags.push_back(createMag(85, 85, 15));
        meds.push_back(createMed(70, 90, 20));
        meds.push_back(createMed(80, 75, 20));

        // sekitar inner block
        mags.push_back(createMag(72, 65, 7));
        meds.push_back(createMed(78, 68, 20));

        // bawah corridor
        mags.push_back(createMag(50, 78, 6));
        mags.push_back(createMag(60, 82, 8));
        meds.push_back(createMed(55, 88, 20));

        for (auto &m : mags)
        {
            m.texture = loadTexture(getAssets("/props/bullet-drop.png"));
        }

        for (auto &m : meds)
        {
            m.texture = loadTexture(getAssets("/props/medkit.png"));
        }

        // outer wall
        addWall(0, 0, 0, 120, 10, 5);
        addWall(0, 0, 0, 5, 10, 120);
        addWall(0, 0, 115, 120, 10, 120);
        addWall(115, 0, 0, 120, 10, 120);

        // left room
        addWall(10, 0, 10, 30, 10, 15);
        addWall(10, 0, 10, 15, 10, 40);
        addWall(10, 0, 35, 30, 10, 40);

        // mid corridor
        addWall(30, 0, 25, 60, 10, 30);
        addWall(55, 0, 25, 60, 10, 60);

        // right room
        addWall(60, 0, 60, 90, 10, 65);
        addWall(60, 0, 60, 65, 10, 100);
        addWall(60, 0, 95, 90, 10, 100);
        addWall(85, 0, 60, 90, 10, 100);

        // inner block
        addWall(70, 0, 70, 80, 10, 90);

        // bottom path
        addWall(30, 0, 60, 55, 10, 65);
        addWall(30, 0, 60, 35, 10, 85);
        addWall(40, 0, 85, 90, 10, 90);

        enemyList.clear();

        int eid = 0;

        // kiri area
        enemyList.push_back(createEnemy(20, 25, eid++));
        enemyList.push_back(createEnemy(22, 30, eid++));

        // tengah corridor
        enemyList.push_back(createEnemy(45, 35, eid++));
        enemyList.push_back(createEnemy(50, 40, eid++));

        // tikungan
        enemyList.push_back(createEnemy(60, 50, eid++));

        // kanan area
        enemyList.push_back(createEnemy(75, 80, eid++));
        enemyList.push_back(createEnemy(80, 85, eid++));
        enemyList.push_back(createEnemy(70, 75, eid++));

        // sekitar tengah block
        enemyList.push_back(createEnemy(75, 65, eid++));

        // bawah corridor
        enemyList.push_back(createEnemy(50, 80, eid++));
        enemyList.push_back(createEnemy(60, 82, eid++));
    }

    void step(float dt) override
    {
        for (auto &bullet : bullets)
        {
            if (bullet.isActive)
            {
                bullet.step++;
                bullet.x += bullet.dirX * 0.1f;
                bullet.z += bullet.dirZ * 0.1f;

                if (checkAllCollisionsBasic(bullet.x, bullet.y, bullet.z))
                {
                    bullet.isActive = false;
                }

                EnemyIsColliding hit = checkEnemyCollisions(bullet.x, bullet.y, bullet.z);
                if (hit.isColliding)
                {
                    bullet.isActive = false;
                    enemyId[hit.id] = true;
                }
            }
        }
    }

    void draw() override
    {
        float b = 0.02f;

        d3d_draw_floor(0, 0, 0, 120, 0, 120, FloorTexture, 20, 20);
        d3d_draw_floor(0, 10, 0, 120, 10, 120, CellingTexture, 20, 20);

        // outer wall
        drawWall(0, 0, 0, 120, 10, 5);
        drawWall(0, 0, 0, 5, 10, 120);
        drawWall(0, 0, 115, 120, 10, 120);
        drawWall(115, 0, 0, 120, 10, 120);

        // left room
        drawWall(10, 0, 10, 30, 10, 15);
        drawWall(10, 0, 10, 15, 10, 40);
        drawWall(10, 0, 35, 30, 10, 40);

        // mid corridor
        drawWall(30, 0, 25, 60, 10, 30);
        drawWall(55, 0, 25, 60, 10, 60);

        // right room
        drawWall(60, 0, 60, 90, 10, 65);
        drawWall(60, 0, 60, 65, 10, 100);
        drawWall(60, 0, 95, 90, 10, 100);
        drawWall(85, 0, 60, 90, 10, 100);

        // inner block
        drawWall(70, 0, 70, 80, 10, 90);

        // bottom path
        drawWall(30, 0, 60, 55, 10, 65);
        drawWall(30, 0, 60, 35, 10, 85);
        drawWall(40, 0, 85, 90, 10, 90);

        for (auto &e : enemyList)
        {
            if (e.isAlive)
            {
                e.draw();
            }
        }
        mag1.draw();
        med1.draw();

        for (auto &m : mags)
        {
            if (!m.isPickedUp)
            {
                m.draw();
            }
        }

        for (auto &m : meds)
        {
            m.draw();
        }
        for (auto &bullet : bullets)
        {
            if (bullet.isActive && bullet.step > 8)
            {
                d3d_draw_ellipsoid(
                    bullet.x - b, bullet.y - b, bullet.z - b,
                    bullet.x + b, bullet.y + b, bullet.z + b,
                    BulletTexture, 1, 1, 20);
            }
        }
    }
};

Map1 map1Instance;
MapSystem::Map *g_map1 = &map1Instance;