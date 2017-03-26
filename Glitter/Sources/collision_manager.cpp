#include "collision_manager.hpp"

#include "world.hpp"

namespace Asteroid {

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
		for (auto it = colliders.begin(); it != colliders.end(); ++it) {
			auto jt = it;
			++jt;
			for (; jt != colliders.end(); ++jt) {
				auto a = *it;
				auto b = *jt;
				if (a->intersects(*b)) {
					a->onCollision(*b);
					b->onCollision(*a);
				}
			}
		}
	}
}
