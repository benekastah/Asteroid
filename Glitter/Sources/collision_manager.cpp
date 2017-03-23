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
		for (auto i = colliders.cbegin(); i != colliders.cend(); ++i) {
			for (auto j = colliders.cbegin(); j != colliders.cend(); ++j) {
				if (i != j) {
					auto a = *i;
					auto b = *j;
					if (a->intersects(*b)) {
						a->onCollision(*b);
						b->onCollision(*a);
					}
				}
			}
		}
	}
}
