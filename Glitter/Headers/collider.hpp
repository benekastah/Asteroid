#pragma once

#include <set>
#include <functional>

#include "glm/vec2.hpp"

#include "rigidbody.hpp"

namespace Asteroid {

	enum ObjectType {
		PLAYER, PROJECTILE, ASTEROID
	};

	class Collider {
	public:
		Collider();
		Collider(Rigidbody * mRb, float mRadius, enum ObjectType mType);
		~Collider();

		Rigidbody * rb;
		float radius;
		enum ObjectType type;

		bool intersects(const Collider coll);
		void onCollision(const Collider coll);
		void addCollisionCallback(std::function<void(const Collider)> cb);

	private:
        std::vector<std::function<void(const Collider)>> collisionCallbacks;
	};
}
