#pragma once
#include "../loader.h"
#include "../globals.h"
#include "../wrapper/d3d.h"

namespace Medkit {
    class medkit
    {
    public:
        GLuint texture = 0;
        float x = 0.0f;
        float y = 0.0f;
        bool isPickedUp = false;
        float healthAmount = 25.0f;

        medkit(float x, float y, float health) {
            this->x = x;
            this->y = y;
            this->healthAmount = health;
            this->isPickedUp = false;
        }
        void draw();
    };
}
