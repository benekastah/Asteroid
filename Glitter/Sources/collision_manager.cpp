#include "collision_manager.hpp"

#include "world.hpp"

namespace Asteroid {

	const float tau = (float) PI * 2;

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

	void CollisionManager::collide() {
		for (auto a : colliders) {
			for (auto b : colliders) {
				if (a != b) {
					if (a->intersects(*b)) {
						a->onCollision(*b);
						b->onCollision(*a);
					}
				}
			}
		}
	}
}
