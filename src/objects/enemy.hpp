#pragma once
#include "../loader.h"
#include "../globals.h"
#include "../wrapper/d3d.h"

namespace Enemy {
    class enemy
    {
    private:
        /* data */
    public:
        GLuint texture = 0;
        float x = 0.1f;     
        float y = 0.1f;       
        bool isAlive = true;
        float speed = 1.0f;
      
        enemy() = default;
        enemy(GLuint texture, float x, float y);
        void draw();
    };
}