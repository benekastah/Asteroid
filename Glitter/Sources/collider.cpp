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

    void Collider::addCollisionCallback(std::function<void(Collider)> cb) {
        collisionCallbacks.push_back(cb);
    }

    //void translatePoints(glm::vec2 * a, glm::vec2 * b) {
    //    auto diff = toVec2(50) - *a;
    //    *a = *a + diff;
    //    *b = *b + diff;
    //}

    //bool circlesIntersect(Rigidbody aRb, Rigidbody bRb) {
    //    auto world = World::getInstance();
    //    glm::vec2 aPos = aRb.pos;
    //    glm::vec2 bPos = bRb.pos;
    //    translatePoints(&aPos, &bPos);
    //    if (aRb.inBounds) {
    //        aPos = world.wrapWorldCoord(aPos);
    //    }
    //    if (bRb.inBounds) {
    //        bPos = world.wrapWorldCoord(bPos);
    //    }
    //    return powf(aPos.x - bPos.x, 2) + powf(aPos.y - bPos.y, 2) <= powf(aRb.radius + bRb.radius, 2);
    //}

    bool Collider::intersects(const Collider coll) {
        auto d = distance(WORLD_SIZE, rb->pos, coll.rb->pos);
        return distance(WORLD_SIZE, rb->pos, coll.rb->pos) < rb->radius + coll.rb->radius;
    }

    bool Collider::interactsWith(const Collider coll) {
        if (type == PROJECTILE && coll.type == PLAYER ||
            type == PLAYER && coll.type == PROJECTILE ||
            type == PROJECTILE && coll.type == PROJECTILE) {
            return false;
        }
        return true;
    }

    void Collider::onCollision(const Collider coll) {
        for each (auto cb in collisionCallbacks) {
            cb(coll);
        }
    }
}
