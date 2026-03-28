/*
 * Copyright (c) 2026 Yohanes Oktanio
 * All rights reserved.
 */

#pragma once
#include <GL/gl.h>
#include <cstdio>
#include <cstring>

#include "../../libs/stb_truetype.h"

struct Font {
    GLuint texture = 0;
    stbtt_bakedchar cdata[96]; 
    unsigned char bitmap[512*512];
};

inline bool loadFont(Font &font, const char* filename, float pixelHeight = 32.0f) {
    FILE* f = fopen(filename, "rb");
    if (!f) return false;
    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    fseek(f, 0, SEEK_SET);
    unsigned char* buffer = new unsigned char[size];
    fread(buffer, 1, size, f);
    fclose(f);

    if (stbtt_BakeFontBitmap(buffer,0, pixelHeight, font.bitmap, 512,512, 32,96,font.cdata) < 0) {
        delete[] buffer;
        return false;
    }

    delete[] buffer;

    glGenTextures(1, &font.texture);
    glBindTexture(GL_TEXTURE_2D, font.texture);
    glTexImage2D(GL_TEXTURE_2D,0,GL_ALPHA,512,512,0,GL_ALPHA,GL_UNSIGNED_BYTE,font.bitmap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return true;
}

inline void renderText(Font &font, float x, float y, const char* text, int windowH, float fontSize = 1.0f) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, font.texture);

    float sx = x;
    float sy = y;

    while (*text) {
        if (*text >= 32 && *text < 128) {
            stbtt_aligned_quad q;
            stbtt_GetBakedQuad(font.cdata, 512, 512, *text - 32, &sx, &sy, &q, 1);

            float x0 = x + (q.x0 - x) * fontSize;
            float x1 = x + (q.x1 - x) * fontSize;
            float y0 = y + (q.y0 - y) * fontSize;
            float y1 = y + (q.y1 - y) * fontSize;

            float fy0 = windowH - y0;
            float fy1 = windowH - y1;

            glBegin(GL_QUADS);
            glTexCoord2f(q.s0, q.t0); glVertex2f(x0, fy0);
            glTexCoord2f(q.s1, q.t0); glVertex2f(x1, fy0);
            glTexCoord2f(q.s1, q.t1); glVertex2f(x1, fy1);
            glTexCoord2f(q.s0, q.t1); glVertex2f(x0, fy1);
            glEnd();
        }
        ++text;
    }

    glDisable(GL_TEXTURE_2D);
}

inline void renderText_2(Font &font, float x, float y, const char* text, float fontSize = 1.0f) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, font.texture);

    float sx = x;
    float sy = y;

    while (*text) {
        if (*text >= 32 && *text < 128) {
            stbtt_aligned_quad q;
            stbtt_GetBakedQuad(font.cdata, 512, 512, *text - 32, &sx, &sy, &q, 1);

            float x0 = x + (q.x0 - x) * fontSize;
            float x1 = x + (q.x1 - x) * fontSize;
            float y0 = y + (q.y0 - y) * fontSize;
            float y1 = y + (q.y1 - y) * fontSize;

            glBegin(GL_QUADS);
                glTexCoord2f(q.s0, q.t0); glVertex2f(x0, y0);
                glTexCoord2f(q.s1, q.t0); glVertex2f(x1, y0);
                glTexCoord2f(q.s1, q.t1); glVertex2f(x1, y1);
                glTexCoord2f(q.s0, q.t1); glVertex2f(x0, y1);
            glEnd();
        }
        ++text;
    }

    glDisable(GL_TEXTURE_2D);
}

inline float getTextWidth(Font &font, const char* text, float scale = 1.0f) {
    float width = 0.0f;
    while (*text) {
        if (*text >= 32 && *text < 128) {
            width += font.cdata[*text - 32].xadvance * scale;
        }
        ++text;
    }
    return width;
}

inline void renderTextCentered(Font &font, float centerX, float y, const char* text, float scale = 1.0f) {
    float w = getTextWidth(font, text, scale);
    renderText_2(font, centerX - w / 2.0f, y, text, scale);
}