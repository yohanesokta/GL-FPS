#include "player.h"
#include "../libs/d3d/d3d.h"
#include <cmath>
#include <stdio.h>
#include <stdlib.h>

bool checkAllCollisions(float x, float y, float z) {
    float pr = 0.5f; 
    if (d3d_collision_block(x, y, z, pr, -10, 0, -10, -5, 5, -5)) return true;
    if (d3d_collision_cylinder(x, y, z, pr, 10, 0, -10, 15, 10, -5)) return true;
    if (d3d_collision_ellipsoid(x, y, z, pr, -15, 5, 10, -5, 15, 20)) return true;
    return false;
}

void updatePlayer() {
    float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    float deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    float moveSpeed = 5.0f * deltaTime * speed;
    float rotSpeed  = 2.0f * deltaTime;

    float nextX = camX;
    float nextZ = camZ;
    if (keys[ESC_KEY]) {
        exit(0);
    }

    if (keys['w']) {
        nextX += lx * moveSpeed;
        nextZ += lz * moveSpeed;
    }
    if (keys['s']) {
        nextX -= lx * moveSpeed;
        nextZ -= lz * moveSpeed;
    }

    if (keys['a']) angle -= rotSpeed;
    if (keys['d']) angle += rotSpeed;
    if (keys['r']) {
        if (!isReloading && magazine > 0 && bullet < 2 ) {
            isReloading = true;
        }
    }
    if (keys[32]) {
        if (!isShooting && bullet >= 0) {
            isShooting = true;
            bullet -= 1;
        }
        if (bullet == 0 && !isShooting && !isReloading && magazine > 0) {
            isReloading = true;
        }
    }

    if (!checkAllCollisions(nextX, camY, camZ)) {
        camX = nextX;
    }
    if (!checkAllCollisions(camX, camY, nextZ)) {
        camZ = nextZ;
    }

    lx = sin(angle);
    lz = -cos(angle);

    // shooting animation
    if (isShooting) {
        shotTime += 0.1f;
        image_index = static_cast<int>(shotTime);
        if (image_index >= 5) {
            image_index = 0;
            isShooting = false;
            shotTime = 0;
        }
    }

    if (bullet < 0 && !isReloading && magazine > 0) {
        isReloading = true;
    }

    if (isReloading) {
        bullet = 0;
        shotTime += 0.1f;
        image_index = 9 + static_cast<int>(shotTime);
        if (image_index >= 15) {
            image_index = 0;
            isReloading = false;
            shotTime = 0;
            bullet = 2;
            magazine -= 1;
        }
    }
}

void handleMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (!isShooting && bullet > 0) {
            isShooting = true;
            bullet -= 1;
        }
    }
}
