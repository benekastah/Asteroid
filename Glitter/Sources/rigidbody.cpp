#include "rigidbody.hpp"

#include "world.hpp"
#include "game_state.hpp"

namespace Asteroid {

	Rigidbody::Rigidbody(float mMass, float mMaxVelocity, glm::vec2 mVelocity, glm::vec2 mPos) {
		initialize(mMass, mMaxVelocity, mVelocity, mPos);
	}

	Rigidbody::Rigidbody(float mMass, glm::vec2 mPos) {
		initialize(mMass, 0, glm::vec2(0, 0) , mPos);
	}

	Rigidbody::Rigidbody() {
		initialize(0, 0, glm::vec2(0, 0) , glm::vec2(0, 0));
	}

	void Rigidbody::applyForce(glm::vec2 force) {
		acc.x = force.x / mass;
		acc.y = force.y / mass;
	}

	void Rigidbody::applyVelocity(glm::vec2 vel) {
		nextVelocity.first = true;
		nextVelocity.second = vel;
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
		pos = World::getInstance().wrapWorldCoord(pos);

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

	void Rigidbody::initialize(float mMass, float mMaxVelocity, glm::vec2 mVelocity, glm::vec2 mPos) {
		mass = mMass;
		maxVelocity = mMaxVelocity;
		velocity = mVelocity;
		pos = mPos;
		acc = glm::vec2(0, 0);
	}
}
