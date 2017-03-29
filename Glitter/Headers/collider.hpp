#pragma once

#include <set>
#include <functional>

#include "glm/vec2.hpp"

#include "rigidbody.hpp"

namespace Asteroid {

    enum ObjectType {
        PLAYER, PROJECTILE, ASTEROID
    };

    class Collider {
    public:
        Collider(Rigidbody * mRb, enum ObjectType mType);
        ~Collider();

        Rigidbody * rb;
        enum ObjectType type;
        bool enabled;

        bool intersects(const Collider coll);
        void onCollision(const Collider coll);
        void addCollisionCallback(std::function<void(const Collider)> cb);
        void enable();
        void disable();

    private:
        std::vector<std::function<void(const Collider)>> collisionCallbacks;
        void applyCollisionForce(const Collider coll);
    };
}
