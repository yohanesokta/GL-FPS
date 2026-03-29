#pragma once

namespace MapSystem {
    class Map {
    public:
        virtual ~Map() {}
        virtual void init() = 0;
        virtual void step(float dt) = 0;
        virtual void draw() = 0;
    };
}
