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

    void translatePoints(glm::vec2 * a, glm::vec2 * b) {
        auto diff = toVec2(50) - *a;
        *a = *a + diff;
        *b = *b + diff;
    }

    bool circlesIntersect(Rigidbody aRb, Rigidbody bRb) {
        auto world = World::getInstance();
        glm::vec2 aPos = aRb.pos;
        glm::vec2 bPos = bRb.pos;
        translatePoints(&aPos, &bPos);
        if (aRb.inBounds) {
            aPos = world.wrapWorldCoord(aPos);
        }
        if (bRb.inBounds) {
            bPos = world.wrapWorldCoord(bPos);
        }
        return powf(aPos.x - bPos.x, 2) + powf(aPos.y - bPos.y, 2) <= powf(aRb.radius + bRb.radius, 2);
    }

    bool Collider::intersects(const Collider coll) {
        return circlesIntersect(*rb, *coll.rb);
    }

    void Collider::applyCollisionForce(const Collider coll) {
        // https://en.wikipedia.org/wiki/Coefficient_of_restitution
        static const float restitution = 3;

        // http://stackoverflow.com/q/345838/777929 provided the following algorithm
        auto p1 = rb->pos;
        auto p2 = coll.rb->pos;
        translatePoints(&p1, &p2);
        if (rb->inBounds) {
            p1 = World::getInstance().wrapWorldCoord(p1);
        }
        if (coll.rb->inBounds) {
            p2 = World::getInstance().wrapWorldCoord(p2);
        }

        auto delta = p1 - p2;
        float d = glm::length(delta);
        if (d == 0) {  // Two positions are the same, handle it
            auto pos = rb->pos;
            if (randfBtwn(0, 1) > 0.5) {
                pos.x += randfBtwn(-1, 1);
            } else {
                pos.y += randfBtwn(-1, 1);
            }
            rb->applyPos(pos);
            // Apply a small velocity to push the two bodies apart.
            auto posDiff = pos - rb->pos;
            rb->applyVelocity((pos - rb->pos) * randVec2(5, 11));
            return;
        }
        auto mtd = delta * toVec2((rb->radius + coll.rb->radius - d) / d);

        float im1 = 1 / rb->mass;
        float im2 = 1 / coll.rb->mass;

        rb->applyPos(rb->pos + mtd * toVec2(im1 / (im1 + im2)));

        auto v = rb->velocity - coll.rb->velocity;
        float vn = glm::dot(v, glm::normalize(mtd));

        if (vn > 0) {
            return;
        }

        float i = -(1 + restitution) * vn / (im1 + im2);
        auto impulse = mtd * toVec2(i);

        rb->applyVelocity(rb->velocity + impulse * im1);
    }

    void Collider::onCollision(const Collider coll) {
        if (type == PROJECTILE && coll.type == PLAYER ||
            type == PLAYER && coll.type == PROJECTILE ||
            type == PROJECTILE && coll.type == PROJECTILE) {
            return;
        }

        applyCollisionForce(coll);

        for each (auto cb in collisionCallbacks) {
            cb(coll);
        }
    }
}
