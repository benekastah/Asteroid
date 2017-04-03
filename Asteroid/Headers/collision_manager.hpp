#pragma once

#include <set>

#include "glm/vec2.hpp"

#include "collider.hpp"

namespace Asteroid {

    class CollisionManager {
    public:
        std::set<Collider *> colliders;

        static CollisionManager & getInstance();
        void add(Collider * coll);
        void remove(Collider * coll);
        void collide();

    private:
        void applyCollisionForce(Collider * a, Collider * b);
    };
}
