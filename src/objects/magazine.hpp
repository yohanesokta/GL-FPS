#pragma once
#include "../loader.h"
#include "../globals.h"
#include "../wrapper/d3d.h"

namespace Magazine {
    class magazine
    {
    public:
        GLuint texture = 0;
        float x = 0.0f;
        float y = 0.0f;
        bool isPickedUp = false;
        int ammoAmount = 5;

        magazine(float x, float y, int ammo) {
            this->x = x;
            this->y = y;
            this->ammoAmount = ammo;
            this->isPickedUp = false;
        }
        void draw();
    };
}
