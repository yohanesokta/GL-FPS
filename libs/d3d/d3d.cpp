#include "d3d.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void d3d_draw_floor(float x1, float y1, float z1, float x2, float y2, float z2, GLuint tex, float hrepeat, float vrepeat) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0);               glVertex3f(x1, y1, z1);
        glTexCoord2f(hrepeat, 0);         glVertex3f(x2, y1, z1);
        glTexCoord2f(hrepeat, vrepeat);   glVertex3f(x2, y1, z2);
        glTexCoord2f(0, vrepeat);         glVertex3f(x1, y1, z2);
    glEnd();
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
