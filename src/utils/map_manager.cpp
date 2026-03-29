#include "map_manager.hpp"
#include <cstddef>

namespace MapSystem {
    static Map* activeMap = nullptr;

    void load(Map* newMap) {
        if (newMap) {
            activeMap = newMap;
            activeMap->init();
        }
    }

    void update(float dt) {
        if (activeMap) {
            activeMap->step(dt);
        }
    }

    void render() {
        if (activeMap) {
            activeMap->draw();
        }
    }
}
