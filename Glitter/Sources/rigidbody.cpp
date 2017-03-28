#include "rigidbody.hpp"

#include "world.hpp"
#include "game_state.hpp"

namespace Asteroid {

    bool pointInBounds(glm::vec2 pt) {
        return pt.x >= 0 && pt.x < WORLD_WIDTH && pt.y >= 0 && pt.y < WORLD_HEIGHT;
    }

    bool circleInBounds(glm::vec2 pt, float r) {
        static unsigned int numAngles = 4;
        static float totalRadians = 2 * PI;
        static float advanceBy = totalRadians / numAngles;
        float dir = 0;
        for (int i = 0; i < numAngles; i++) {
            if (!pointInBounds(pointOnCircle(pt, r, dir))) {
                return false;
            }
            dir += advanceBy;
        }
        return true;
    }

    Rigidbody::Rigidbody(float mMass, float mMaxVelocity, glm::vec2 mVelocity, glm::vec2 mPos, float mRadius) {
        initialize(mMass, mMaxVelocity, mVelocity, mPos, mRadius);
    }

    Rigidbody::Rigidbody(float mMass, glm::vec2 mPos, float mRadius) {
        initialize(mMass, 0, glm::vec2(0, 0), mPos, mRadius);
    }

    Rigidbody::Rigidbody() {
        initialize(0, 0, glm::vec2(0, 0) , glm::vec2(0, 0), 0);
    }

    void Rigidbody::applyForce(glm::vec2 force) {
        acc.x = force.x / mass;
        acc.y = force.y / mass;
    }

    void Rigidbody::applyVelocity(glm::vec2 vel) {
        if (nextVelocity.first) {
            nextVelocity.second += vel;
            nextVelocity.second /= toVec2(2);
        } else {
            nextVelocity.first = true;
            nextVelocity.second = vel;
        }
    }

    void Rigidbody::applyPos(glm::vec2 mPos) {
        nextPos.first = true;
        nextPos.second = mPos;
    }

    void Rigidbody::step(GameState state, double t, double dt) {
        if (nextVelocity.first) {
            nextVelocity.first = false;
            velocity = nextVelocity.second;
        }
        if (nextPos.first) {
            nextPos.first = false;
            pos = nextPos.second;
        }
        pos.x += velocity.x * dt + 0.5 * acc.x * powf(dt, 2);
        pos.y += velocity.y * dt + 0.5 * acc.y * powf(dt, 2);
        if (inBounds) {
            pos = World::getInstance().wrapWorldCoord(pos);
        } else {
            // Once in bounds for the first time, the object remains in bounds.
            inBounds = circleInBounds(pos, radius);
        }

        velocity.x += acc.x * dt;
        velocity.y += acc.y * dt;
        acc = glm::vec2(0, 0);

        if (maxVelocity > 0) {
            if (velocity.x < -maxVelocity) { velocity.x = -maxVelocity; }
            if (velocity.x > maxVelocity) { velocity.x = maxVelocity; }
            if (velocity.y < -maxVelocity) { velocity.y = -maxVelocity; }
            if (velocity.y > maxVelocity) { velocity.y = maxVelocity; }
        }
    }

    void Rigidbody::initialize(float mMass, float mMaxVelocity, glm::vec2 mVelocity, glm::vec2 mPos, float mRadius) {
        mass = mMass;
        maxVelocity = mMaxVelocity;
        velocity = mVelocity;
        pos = mPos;
        radius = mRadius;
        acc = glm::vec2(0, 0);
        inBounds = circleInBounds(pos, radius);
    }
}
