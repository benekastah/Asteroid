#include "collider.hpp"

namespace Asteroid {

	void ColliderManager::add(Collider * coll) {
		colliders.insert(coll);
	}

	void ColliderManager::remove(Collider * coll) {
		colliders.erase(coll);
	}

	std::vector<std::tuple<Collider, Collider>> ColliderManager::getCollisions() {
		return std::vector<std::tuple<Collider, Collider>>();
	}

	Collider::Collider(Rigidbody * mRb, float mSize) {
		rb = mRb;
		size = mSize;
		ColliderManager::add(this);
	}

	Collider::~Collider() {
		ColliderManager::remove(this);
	}
}
