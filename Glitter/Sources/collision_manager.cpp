#include "collision_manager.hpp"

#include "world.hpp"

namespace Asteroid {

    static const glm::vec2 WORLD_SIZE = World::getInstance().size();

    CollisionManager & CollisionManager::getInstance() {
        static CollisionManager manager;
        return manager;
    }

    void CollisionManager::add(Collider * coll) {
        colliders.insert(coll);
    }

    void CollisionManager::remove(Collider * coll) {
        colliders.erase(coll);
    }

    void CollisionManager::applyCollisionForce(Collider * a, Collider * b) {
        // https://en.wikipedia.org/wiki/Coefficient_of_restitution
        static const float restitution = 1;

        // http://stackoverflow.com/q/345838/777929 provided the following algorithm
        auto delta = subtract(WORLD_SIZE, a->rb->pos, b->rb->pos);
        float d = glm::length(delta);
        if (d == 0) {  // Two positions are the same, handle it
            auto pos = a->rb->pos;
            if (randfBtwn(0, 1) > 0.5) {
                pos.x += randfBtwn(-1, 1);
            } else {
                pos.y += randfBtwn(-1, 1);
            }
            a->rb->applyPos(pos);
            // Apply a small velocity to push the two bodies apart.
            auto posDiff = pos - a->rb->pos;
            a->rb->applyVelocity((pos - a->rb->pos) * randVec2(5, 11));
            return;
        }
        auto mtd = delta * toVec2((a->rb->radius + b->rb->radius - d) / d);

        float im1 = 1 / a->rb->mass;
        float im2 = 1 / b->rb->mass;

        auto aNextPos = a->rb->pos + mtd * toVec2(im1 / (im1 + im2));
        a->rb->applyPos(aNextPos);
        auto bNextPos = b->rb->pos - mtd * toVec2(im2 / (im1 + im2));
        b->rb->applyPos(bNextPos);

        {
            auto aRb = Rigidbody(*a->rb);
            auto bRb = Rigidbody(*b->rb);
            aRb.pos = aNextPos;
            bRb.pos = bNextPos;
            auto a2 = Collider(&aRb, a->type);
            auto b2 = Collider(&bRb, b->type);
            if (a2.intersects(b2)) {
                auto onePlusOne = "five";
            }
        }

        auto v = a->rb->velocity - b->rb->velocity;
        float vn = glm::dot(v, glm::normalize(mtd));

        if (vn > 0) {
            return;
        }

        float i = -(1 + restitution) * vn / (im1 + im2);
        auto impulse = glm::normalize(mtd) * toVec2(i);

        a->rb->applyVelocity(a->rb->velocity + impulse * im1);
        b->rb->applyVelocity(b->rb->velocity - impulse * im2);
    }

    void CollisionManager::collide() {
        for (auto it = colliders.begin(); it != colliders.end(); ++it) {
            auto jt = it;
            for (++jt; jt != colliders.end(); ++jt) {
                auto a = *it;
                auto b = *jt;
                if (a->interactsWith(*b) && a->intersects(*b)) {
                    applyCollisionForce(a, b);
                    a->onCollision(*b);
                    b->onCollision(*a);
                }
            }
        }
    }
}
