/*
 * Copyright (c) 2026 Yohanes Oktanio
 * All rights reserved.
 */

// font.hpp
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

inline void renderText(Font &font, float x, float y, const char* text, int windowH) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, font.texture);
    float sx = x;
    float sy = y;

    while (*text) {
        if (*text >= 32 && *text < 128) {
            stbtt_aligned_quad q;
            stbtt_GetBakedQuad(font.cdata, 512,512, *text-32, &sx,&sy, &q, 1);

            float y0 = windowH - q.y0;
            float y1 = windowH - q.y1;

            glBegin(GL_QUADS);
            glTexCoord2f(q.s0,q.t0); glVertex2f(q.x0,y0);
            glTexCoord2f(q.s1,q.t0); glVertex2f(q.x1,y0);
            glTexCoord2f(q.s1,q.t1); glVertex2f(q.x1,y1);
            glTexCoord2f(q.s0,q.t1); glVertex2f(q.x0,y1);
            glEnd();
        }
        ++text;
    }
    glDisable(GL_TEXTURE_2D);
}