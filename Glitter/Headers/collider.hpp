#pragma once

#include <set>
#include <tuple>

#include "glm/vec2.hpp"

#include "game_object.hpp"
#include "rigidbody.hpp"

namespace Asteroid {
	class ColliderManager {
	public:
		static std::set<Collider *> colliders;

		static void add(Collider * coll);
		static void remove(Collider * rb);

		std::vector<std::tuple<Collider, Collider>> getCollisions();
	};

	class Collider : GameObject {
	public:
		Collider(Rigidbody * mRb, float mSize);
		~Collider();

		Rigidbody * rb;
		float size;
	};
}
