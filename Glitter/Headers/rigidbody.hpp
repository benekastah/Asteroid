#pragma once

#include "glm/vec2.hpp"

#include "game_object.hpp"

namespace Asteroid {
    class GameState;

    class Rigidbody : GameObject {
    public:
        Rigidbody();
        Rigidbody(float mMass, float mMaxVelocity, glm::vec2 mVelocity, glm::vec2 mPos, float mRadius);
        Rigidbody(float mMass, glm::vec2 mPos, float mRadius);

        void applyForce(glm::vec2 force);
        void applyVelocity(glm::vec2 vel);
        void applyPos(glm::vec2 pos);
        void step(GameState state, double t, double dt);

        float mass;
        float maxVelocity;
        float radius;
        glm::vec2 velocity;
        glm::vec2 pos;
        bool inBounds;

    private:
        void initialize(float mMass, float mMaxVelocity, glm::vec2 mVelocity, glm::vec2 mPos, float mRadius);

        glm::vec2 acc;
        std::pair<bool, glm::vec2> nextVelocity;
        std::pair<bool, glm::vec2> nextPos;
    };
}
