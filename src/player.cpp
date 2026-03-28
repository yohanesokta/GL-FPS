#include "player.h"
#include "wrapper/d3d.h"
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include "wrapper/audio.hpp"

void playerShoot()
{
    Audio::Manager::playSound(getAssets("/sound/shoot-p.wav"));
    isShooting = true;
    bullet -= 1;
    bullets.push_back({camX, camY, camZ, lx, 0, lz, 0, true});
}

void updatePlayer()
{
    float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    float deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    float moveSpeed = 5.0f * deltaTime * speed;
    float rotSpeed = 2.0f * deltaTime;

    float nextX = camX;
    float nextZ = camZ;
    if (keys[ESC_KEY])
    {
        exit(0);
    }

    if (keys['w'])
    {
        nextX += lx * moveSpeed;
        nextZ += lz * moveSpeed;
    }
    if (keys['s'])
    {
        nextX -= lx * moveSpeed;
        nextZ -= lz * moveSpeed;
    }

    if (keys['a'])
    {
        nextX += lz * (moveSpeed / 2);
        nextZ -= lx * (moveSpeed / 2);
    }
    if (keys['d'])
    {
        nextX -= lz * (moveSpeed / 2);
        nextZ += lx * (moveSpeed / 2);
    }
    if (special[GLUT_KEY_LEFT])
    {
        angle -= rotSpeed;
    }
    if (special[GLUT_KEY_RIGHT])
    {
        angle += rotSpeed;
    }

    if (keys['r'])
    {
        if (!isReloading && magazine > 0 && bullet < 10)
        {
            isReloading = true;
        }
    }
    if (keys[32])
    {
        if (!isShooting && bullet > 0)
        {
            playerShoot();
        }
        if (bullet == 0 && !isShooting && !isReloading && magazine > 0)
        {
            isReloading = true;
        }
    }

    if (!checkAllCollisionsBasic(nextX, camY, camZ))
    {
        camX = nextX;
    }
    if (!checkAllCollisionsBasic(camX, camY, nextZ))
    {
        camZ = nextZ;
    }

    lx = sin(angle);
    lz = -cos(angle);

    if (isShooting)
    {
        shotTime += deltaTime * 20.0f;
        image_index = 35 + static_cast<int>(shotTime);
        if (image_index >= 39)
        {
            image_index = 0;
            isShooting = false;
            shotTime = 0;
        }
    }

    if (bullet < 0 && !isReloading && magazine > 0)
    {
        isReloading = true;
    }

    if (isReloading)
    {
        if (shotTime == 0)
        {
            Audio::Manager::playSound(getAssets("/sound/reload-p.wav"));
        }
        bullet = 0;
        shotTime += deltaTime * 20.0f;
        image_index = 1 + static_cast<int>(shotTime);
        if (image_index >= 34)
        {
            image_index = 0;
            isReloading = false;
            shotTime = 0;
            bullet = MAX_AMMO;
            magazine -= 1;
        }
    }

    //   d3d_draw_wall_rot(-14, 5, -9, -14+thickness, 1, -5, NodPropsTexture, 1, 1, get_yaw_to_player(-14, -9, camX, camZ)+90.0f);
    if (d3d_collision_block(camX, camY, camZ, 0.5f, -14, 5, -9, -14 + 0.1f, 1, -5))
    {
        if (!isMusicPlaying)
        {
            printf("Player collided with the nod block! Playing music...\n");
            Audio::Manager::playMusic(getAssets("/sound/cucak-rowo-x-hayangkawin-shessssss.ogg"), true);
            isMusicPlaying = true;
        }
    }
}

void handleMouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if (!isShooting && bullet > 0)
        {
            playerShoot();
        }
        if (bullet == 0 && !isShooting && !isReloading && magazine > 0)
        {
            isReloading = true;
        }
    }
}
