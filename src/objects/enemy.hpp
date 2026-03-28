#pragma once
#include "../loader.h"
#include "../globals.h"
#include "../wrapper/d3d.h"

namespace Enemy {
    class enemy
    {
    private:

    public:
        GLuint texture = 0;
        float x = 0.1f;     
        float y = 0.1f;
        float nextX = 0.1f;
        float nextY = 0.1f;       
        bool isAlive = true;
        bool isHit = false;
        float speed = 1.0f;
        int id;
      
        enemy(float x, float y,int id) {
            this->x = x;
            this->y = y;
            this->id = id;
            this->nextX = x;
            this->nextY = y;
            this->isHit = false;
            this->isAlive = true;
        }
        enemy(GLuint texture, float x, float y);
        void draw();
    };
}