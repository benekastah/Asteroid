#include "collider.hpp"
#include "collision_manager.hpp"
#include "util.hpp"
#include "world.hpp"

namespace Asteroid {

    static const glm::vec2 WORLD_SIZE = World::getInstance().size();

    Collider::Collider(Rigidbody * mRb, enum ObjectType mType) {
        rb = mRb;
        type = mType;
        enabled = false;
    }

    Collider::~Collider() {
        if (enabled) {
            disable();
        }
    }

    void Collider::enable() {
        if (!enabled) {
            enabled = true;
            CollisionManager::getInstance().add(this);
        }
    }

    void Collider::disable () {
        if (enabled) {
            enabled = false;
            CollisionManager::getInstance().remove(this);
        }
    }

    void Collider::addCollisionCallback(std::function<void(const Collider, const InteractionType)> cb) {
        collisionCallbacks.push_back(cb);
    }

    bool Collider::intersects(const Collider coll) {
        auto d = distance(WORLD_SIZE, rb->pos, coll.rb->pos);
        return distance(WORLD_SIZE, rb->pos, coll.rb->pos) < rb->radius + coll.rb->radius;
    }

    InteractionType Collider::getInteractionType(const Collider coll) {
        if (type == PROJECTILE && coll.type == PLAYER ||
            type == PLAYER && coll.type == PROJECTILE ||
            type == PROJECTILE && coll.type == PROJECTILE) {
            return NONE;
        }
        if (type == CROSSHAIR || coll.type == CROSSHAIR) {
            return DETECT;
        }
        return COLLIDE;
    }

    void Collider::onCollision(const Collider coll, const InteractionType t) {
        for each (auto cb in collisionCallbacks) {
            cb(coll, t);
        }
    }
}
