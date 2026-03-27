/*
 * Copyright (c) 2026 Yohanes Oktanio
 * All rights reserved.
 */

#include "d3d.h"
#include <algorithm>
#include <stdio.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Helper function to clamp a value
static float clamp(float n, float lower, float upper) {
    return std::max(lower, std::min(n, upper));
}

void d3d_draw_floor(float x1, float y1, float z1,
                    float x2, float y2, float z2,
                    GLuint tex,
                    float hrepeat, float vrepeat) {

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    int xTiles = (int)hrepeat;
    int zTiles = (int)vrepeat;

    float dx = (x2 - x1) / xTiles;
    float dz = (z2 - z1) / zTiles;

    for (int i = 0; i < xTiles; i++) {
        for (int j = 0; j < zTiles; j++) {

            float xa = x1 + i * dx;
            float xb = xa + dx;

            float za = z1 + j * dz;
            float zb = za + dz;

            glBegin(GL_QUADS);
                glNormal3f(0, 1, 0);

                glTexCoord2f(0, 0); glVertex3f(xa, y1, za);
                glTexCoord2f(1, 0); glVertex3f(xb, y1, za);
                glTexCoord2f(1, 1); glVertex3f(xb, y1, zb);
                glTexCoord2f(0, 1); glVertex3f(xa, y1, zb);

            glEnd();
        }
    }

    glDisable(GL_TEXTURE_2D);
}

void d3d_draw_wall(float x1, float y1, float z1, float x2, float y2, float z2, GLuint tex, float hrepeat, float vrepeat) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0);               glVertex3f(x1, y2, z1);
        glTexCoord2f(hrepeat, 0);         glVertex3f(x2, y2, z2);
        glTexCoord2f(hrepeat, vrepeat);   glVertex3f(x2, y1, z2);
        glTexCoord2f(0, vrepeat);         glVertex3f(x1, y1, z1);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void d3d_draw_block(float x1, float y1, float z1, float x2, float y2, float z2, GLuint tex, float hrepeat, float vrepeat) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);
    glBegin(GL_QUADS);
        // Top
        glTexCoord2f(0, 0);               glVertex3f(x1, y2, z1);
        glTexCoord2f(hrepeat, 0);         glVertex3f(x2, y2, z1);
        glTexCoord2f(hrepeat, vrepeat);   glVertex3f(x2, y2, z2);
        glTexCoord2f(0, vrepeat);         glVertex3f(x1, y2, z2);
        // Bottom
        glTexCoord2f(0, 0);               glVertex3f(x1, y1, z1);
        glTexCoord2f(hrepeat, 0);         glVertex3f(x2, y1, z1);
        glTexCoord2f(hrepeat, vrepeat);   glVertex3f(x2, y1, z2);
        glTexCoord2f(0, vrepeat);         glVertex3f(x1, y1, z2);
        // Front
        glTexCoord2f(0, 0);               glVertex3f(x1, y2, z2);
        glTexCoord2f(hrepeat, 0);         glVertex3f(x2, y2, z2);
        glTexCoord2f(hrepeat, vrepeat);   glVertex3f(x2, y1, z2);
        glTexCoord2f(0, vrepeat);         glVertex3f(x1, y1, z2);
        // Back
        glTexCoord2f(0, 0);               glVertex3f(x1, y2, z1);
        glTexCoord2f(hrepeat, 0);         glVertex3f(x2, y2, z1);
        glTexCoord2f(hrepeat, vrepeat);   glVertex3f(x2, y1, z1);
        glTexCoord2f(0, vrepeat);         glVertex3f(x1, y1, z1);
        // Left
        glTexCoord2f(0, 0);               glVertex3f(x1, y2, z1);
        glTexCoord2f(hrepeat, 0);         glVertex3f(x1, y2, z2);
        glTexCoord2f(hrepeat, vrepeat);   glVertex3f(x1, y1, z2);
        glTexCoord2f(0, vrepeat);         glVertex3f(x1, y1, z1);
        // Right
        glTexCoord2f(0, 0);               glVertex3f(x2, y2, z1);
        glTexCoord2f(hrepeat, 0);         glVertex3f(x2, y2, z2);
        glTexCoord2f(hrepeat, vrepeat);   glVertex3f(x2, y1, z2);
        glTexCoord2f(0, vrepeat);         glVertex3f(x2, y1, z1);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void d3d_draw_cylinder(float x1, float y1, float z1, float x2, float y2, float z2, GLuint tex, float hrepeat, float vrepeat, bool closed, int steps) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);
    
    float radiusX = (x2 - x1) / 2.0f;
    float radiusZ = (z2 - z1) / 2.0f;
    float centerX = (x1 + x2) / 2.0f;
    float centerZ = (z1 + z2) / 2.0f;

    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= steps; i++) {
        float angle = 2.0f * M_PI * i / steps;
        float dx = cos(angle);
        float dz = sin(angle);
        
        glTexCoord2f((float)i / steps * hrepeat, 0);
        glVertex3f(centerX + dx * radiusX, y2, centerZ + dz * radiusZ);
        
        glTexCoord2f((float)i / steps * hrepeat, vrepeat);
        glVertex3f(centerX + dx * radiusX, y1, centerZ + dz * radiusZ);
    }
    glEnd();

    if (closed) {
        // Top cap
        glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(hrepeat * 0.5f, vrepeat * 0.5f);
        glVertex3f(centerX, y2, centerZ);
        for (int i = 0; i <= steps; i++) {
            float angle = 2.0f * M_PI * i / steps;
            glTexCoord2f(hrepeat * (0.5f + 0.5f * cos(angle)), vrepeat * (0.5f + 0.5f * sin(angle)));
            glVertex3f(centerX + cos(angle) * radiusX, y2, centerZ + sin(angle) * radiusZ);
        }
        glEnd();

        // Bottom cap
        glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(hrepeat * 0.5f, vrepeat * 0.5f);
        glVertex3f(centerX, y1, centerZ);
        for (int i = steps; i >= 0; i--) {
            float angle = 2.0f * M_PI * i / steps;
            glTexCoord2f(hrepeat * (0.5f + 0.5f * cos(angle)), vrepeat * (0.5f + 0.5f * sin(angle)));
            glVertex3f(centerX + cos(angle) * radiusX, y1, centerZ + sin(angle) * radiusZ);
        }
        glEnd();
    }
    glDisable(GL_TEXTURE_2D);
}

void d3d_draw_ellipsoid(float x1, float y1, float z1, float x2, float y2, float z2, GLuint tex, float hrepeat, float vrepeat, int steps) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);

    float rx = (x2 - x1) / 2.0f;
    float ry = (y2 - y1) / 2.0f;
    float rz = (z2 - z1) / 2.0f;
    float cx = (x1 + x2) / 2.0f;
    float cy = (y1 + y2) / 2.0f;
    float cz = (z1 + z2) / 2.0f;

    for (int i = 0; i < steps; i++) {
        float lat0 = M_PI * (-0.5f + (float)i / steps);
        float z0 = sin(lat0);
        float zr0 = cos(lat0);

        float lat1 = M_PI * (-0.5f + (float)(i + 1) / steps);
        float z1_ = sin(lat1);
        float zr1 = cos(lat1);

        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= steps; j++) {
            float lng = 2.0f * M_PI * (float)j / steps;
            float x = cos(lng);
            float y = sin(lng);

            glTexCoord2f((float)j / steps * hrepeat, (float)i / steps * vrepeat);
            glVertex3f(cx + x * zr0 * rx, cy + z0 * ry, cz + y * zr0 * rz);
            
            glTexCoord2f((float)j / steps * hrepeat, (float)(i + 1) / steps * vrepeat);
            glVertex3f(cx + x * zr1 * rx, cy + z1_ * ry, cz + y * zr1 * rz);
        }
        glEnd();
    }
    glDisable(GL_TEXTURE_2D);
}

// Simple collision detection (AABB vs Sphere)
bool d3d_collision_block(float px, float py, float pz, float pr, float x1, float y1, float z1, float x2, float y2, float z2) {
    float minX = std::min(x1, x2);
    float maxX = std::max(x1, x2);
    float minY = std::min(y1, y2);
    float maxY = std::max(y1, y2);
    float minZ = std::min(z1, z2);
    float maxZ = std::max(z1, z2);

    float closestX = clamp(px, minX, maxX);
    float closestY = clamp(py, minY, maxY);
    float closestZ = clamp(pz, minZ, maxZ);

    float dx = px - closestX;
    float dy = py - closestY;
    float dz = pz - closestZ;

    return (dx * dx + dy * dy + dz * dz) < (pr * pr);
}

// Cylinder collision detection (Vertical Cylinder vs Sphere)
bool d3d_collision_cylinder(float px, float py, float pz, float pr, float x1, float y1, float z1, float x2, float y2, float z2) {
    float centerX = (x1 + x2) / 2.0f;
    float centerZ = (z1 + z2) / 2.0f;
    float radiusX = std::abs(x2 - x1) / 2.0f;
    float radiusZ = std::abs(z2 - z1) / 2.0f;
    float minY = std::min(y1, y2);
    float maxY = std::max(y1, y2);

    // XZ distance (Elliptical support)
    float dx = (px - centerX) / (radiusX + pr);
    float dz = (pz - centerZ) / (radiusZ + pr);
    
    if (dx * dx + dz * dz < 1.0f) {
        if (py + pr > minY && py - pr < maxY) {
            return true;
        }
    }
    return false;
}

// Ellipsoid collision detection (Simplified to sphere check)
bool d3d_collision_ellipsoid(float px, float py, float pz, float pr, float x1, float y1, float z1, float x2, float y2, float z2) {
    float centerX = (x1 + x2) / 2.0f;
    float centerY = (y1 + y2) / 2.0f;
    float centerZ = (z1 + z2) / 2.0f;
    float rx = std::abs(x2 - x1) / 2.0f;
    float ry = std::abs(y2 - y1) / 2.0f;
    float rz = std::abs(z2 - z1) / 2.0f;

    float dx = (px - centerX) / (rx + pr);
    float dy = (py - centerY) / (ry + pr);
    float dz = (pz - centerZ) / (rz + pr);

    return (dx * dx + dy * dy + dz * dz) < 1.0f;
}

