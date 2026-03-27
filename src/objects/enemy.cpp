#include "enemy.hpp"
namespace Enemy {
    enemy::enemy(GLuint texture, float x, float y)
    {
        this->texture = texture;
        this->x = x;
        this->y = y;
        this->isAlive = true;
    }
    
    void enemy::draw()
    {
        if (this->isAlive ) {
            float direction = get_yaw_to_player(this->x, this->y, camX, camZ);
            d3d_draw_wall_rot(this->x, 1.5f*3.0f, this->y, this->x + 1.5f*3.0f, 0, this->y, this->texture, 1, 1, direction);
            float radiant = direction * (3.14159f / 180.0f);
            this->x += sin(radiant) * speed * 0.01f;
            this->y += cos(radiant) * speed * 0.01f;
        }
    }
}