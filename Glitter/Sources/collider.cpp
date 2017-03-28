#include "collider.hpp"
#include "collision_manager.hpp"
#include "util.hpp"
#include "world.hpp"

namespace Asteroid {

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

    bool circlesIntersect(Rigidbody aRb, Rigidbody bRb) {
        auto diff = toVec2(50) - aRb.pos;
        auto world = World::getInstance();
        auto aPos = aRb.pos + diff;
        auto bPos = bRb.pos + diff;
        if (aRb.inBounds) {
            aPos = world.wrapWorldCoord(aPos);
        }
        if (bRb.inBounds) {
            bPos = world.wrapWorldCoord(bPos);
        }
        return powf(aPos.x - bPos.x, 2) + powf(aPos.y - bPos.y, 2) <= powf(aRb.radius + bRb.radius, 2);
    }

    glm::vec2 distance(glm::vec2 aPos, glm::vec2 bPos) {
        auto diff = toVec2(50) - aPos;
        auto world = World::getInstance();
        return world.wrapWorldCoord(aPos + diff) - world.wrapWorldCoord(bPos + diff);
    }

    bool Collider::intersects(const Collider coll) {
        return circlesIntersect(*rb, *coll.rb);
    }

    void Collider::onCollision(const Collider coll) {
        if (type == PROJECTILE && coll.type == PLAYER ||
            type == PLAYER && coll.type == PROJECTILE ||
            type == PROJECTILE && coll.type == PROJECTILE) {
            return;
        }

        auto totalMass = rb->mass + coll.rb->mass;
        rb->applyVelocity(glm::vec2(
            (rb->velocity.x * (rb->mass - coll.rb->mass) + 2 * coll.rb->mass * coll.rb->velocity.x) / totalMass,
            (rb->velocity.y * (rb->mass - coll.rb->mass) + 2 * coll.rb->mass * coll.rb->velocity.y) / totalMass));

        // Don't allow intersections that are too deep
        auto innerRb = *rb;
        innerRb.radius -= 0.5f;
        if (circlesIntersect(innerRb, *coll.rb)) {
            auto pos = rb->pos + distance(rb->pos, coll.rb->pos) / toVec2(2);
            if (pos == rb->pos) {
                if (randfBtwn(0, 1) > 0.5) {
                    pos.x += randfBtwn(-1, 1);
                } else {
                    pos.y += randfBtwn(-1, 1);
                }
            }
            rb->applyPos(pos);
            // Apply a small velocity to push the two bodies apart.
            auto posDiff = pos - rb->pos;
            rb->applyVelocity((pos - rb->pos) * randVec2(5, 11));
        }

        for each (auto cb in collisionCallbacks) {
            cb(coll);
        }
    }
}
