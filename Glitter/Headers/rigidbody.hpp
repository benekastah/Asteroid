#pragma once

#include "glm/vec2.hpp"

namespace Asteroid {
	class GameState;

	class Rigidbody {
	public:
		Rigidbody();
		Rigidbody(float mMass, float mMaxVelocity, glm::vec2 mVelocity, glm::vec2 mPos);
		Rigidbody(float mMass, glm::vec2 mPos);

		void applyForce(glm::vec2 force);
		void step(GameState state, double t, double dt);

        float mass;
		float maxVelocity;
        glm::vec2 velocity;
        glm::vec2 pos;

	private:
		void initialize(float mMass, float mMaxVelocity, glm::vec2 mVelocity, glm::vec2 mPos);

		glm::vec2 acc;
	};
}
