#include "collider.hpp"
#include "collision_manager.hpp"
#include "util.hpp"
#include "world.hpp"

namespace Asteroid {

    Collider::Collider(Rigidbody * mRb, float mRadius, enum ObjectType mType) {
        rb = mRb;
        radius = mRadius;
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

    bool circlesIntersect(glm::vec2 aPos, float aR, glm::vec2 bPos, float bR) {
        auto diff = toVec2(50) - aPos;
        auto world = World::getInstance();
        aPos = world.wrapWorldCoord(aPos + diff);
        bPos = world.wrapWorldCoord(bPos + diff);
        return powf(aPos.x - bPos.x, 2) + powf(aPos.y - bPos.y, 2) <= powf(aR + bR, 2);
    }

    glm::vec2 distance(glm::vec2 aPos, glm::vec2 bPos) {
        auto diff = toVec2(50) - aPos;
        auto world = World::getInstance();
        return world.wrapWorldCoord(aPos + diff) - world.wrapWorldCoord(bPos + diff);
    }

    bool Collider::intersects(const Collider coll) {
        return circlesIntersect(rb->pos, radius, coll.rb->pos, coll.radius);
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
        if (circlesIntersect(rb->pos, radius - 0.5f, coll.rb->pos, coll.radius)) {
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
