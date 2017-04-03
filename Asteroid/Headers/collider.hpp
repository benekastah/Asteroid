#pragma once

#include <set>
#include <functional>

#include "glm/vec2.hpp"

#include "rigidbody.hpp"

namespace Asteroid {

    enum ObjectType {
        PLAYER, PROJECTILE, ASTEROID, CROSSHAIR
    };

    enum InteractionType {
        NONE, DETECT, COLLIDE
    };

    class Collider {
    public:
        Collider(Rigidbody * mRb, enum ObjectType mType);
        ~Collider();

        Rigidbody * rb;
        ObjectType type;
        bool enabled;

        bool intersects(const Collider coll);
        InteractionType getInteractionType(const Collider coll);
        void onCollision(const Collider coll, const InteractionType t);
        void addCollisionCallback(std::function<void(const Collider, const InteractionType)> cb);
        void enable();
        void disable();

    private:
        std::vector<std::function<void(const Collider, const InteractionType)>> collisionCallbacks;
    };
}
