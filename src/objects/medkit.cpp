#include "medkit.hpp"
#include "../wrapper/audio.hpp"
namespace Medkit {
    void medkit::draw() {
        if (this->isPickedUp) return;
        float direction = get_yaw_to_player(this->x, this->y, camX, camZ);
        float size = 1.0f;
        d3d_draw_block(this->x-size, 0.1f, this->y-size, this->x+size, 0.1f, this->y+size, this->texture, 1, 1 );
        if (d3d_collision_block(camX, camY, camZ, 0.8f, this->x - 0.5f, 0, this->y - 0.5f, this->x + 0.5f, 2.0f, this->y + 0.5f)) {
            ::health = (::health + this->healthAmount > 100.0f) ? 100.0f : ::health + this->healthAmount;
            this->isPickedUp = true;
            Audio::Manager::playSound(getAssets("/sound/pick-medkit.wav"));
        }
    }
}
