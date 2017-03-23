#include "collider.hpp"
#include "collision_manager.hpp"
#include "util.hpp"

namespace Asteroid {

	Collider::Collider() {}

	Collider::Collider(Rigidbody * mRb, float mRadius, enum ObjectType mType) {
		rb = mRb;
		radius = mRadius;
		type = mType;
		CollisionManager::getInstance().add(this);
	}

	Collider::~Collider() {
		CollisionManager::getInstance().remove(this);
	}

	void Collider::addCollisionCallback(std::function<void(Collider)> cb) {
		collisionCallbacks.push_back(cb);
	}

	bool circlesIntersect(glm::vec2 aPos, float aR, glm::vec2 bPos, float bR) {
		return powf(aPos.x - bPos.x, 2) + powf(aPos.y - bPos.y, 2) <= powf(aR + bR, 2);
	}

	bool Collider::intersects(const Collider coll) {
		return circlesIntersect(rb->pos, radius, coll.rb->pos, coll.radius);
	}

	void Collider::onCollision(const Collider coll) {
		if (type == PROJECTILE && coll.type == PLAYER || type == PLAYER && coll.type == PROJECTILE) {
			return;
		}

		auto totalMass = rb->mass + coll.rb->mass;
		rb->applyVelocity(glm::vec2(
			(rb->velocity.x * (rb->mass - coll.rb->mass) + 2 * coll.rb->mass * coll.rb->velocity.x) / totalMass,
			(rb->velocity.y * (rb->mass - coll.rb->mass) + 2 * coll.rb->mass * coll.rb->velocity.y) / totalMass));

		// Don't allow intersections that are too deep
		if (circlesIntersect(rb->pos, radius - 0.5f, coll.rb->pos, coll.radius)) {
			rb->applyPos(rb->pos + (rb->pos - coll.rb->pos));
		}

		for each (auto cb in collisionCallbacks) {
			cb(coll);
		}
	}
}
