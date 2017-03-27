#include "gun.hpp"

#include "game_state.hpp"

namespace Asteroid {

	Gun::Gun() {
		bulletMass = 25;
		bulletForce = 125000;
		maxBullets = 4;
		bulletTimeToLive = 1;
		bulletsPerSecond = 7;
		cooldownRate = 0;
	}

	Gun::~Gun() {
		clearBullets();
	}

	void Gun::clearBullets() {
		for each (auto bullet in bullets) {
			delete bullet;
		}
		bullets.clear();
	}

    bool Gun::fireBullet(double t, glm::vec2 vel, glm::vec2 pos) {
        static double lastFiredBulletAt = 0;
        static double maxedOutAt = 0;
        if (t - lastFiredBulletAt < 1 / bulletsPerSecond) {
            return false;
        }
        if (bullets.size() >= maxBullets) {
            maxedOutAt = t;
            return false;
        }
        if (t - maxedOutAt < cooldownRate) {
            return false;
        }
        lastFiredBulletAt = t;
        auto bullet = new Projectile(bulletMass, bulletForce, pos, vel);
        bullets.push_back(bullet);
		return true;
    }

    void Gun::step(GameState state, double t, double dt) {
        std::vector<int> toDelete;
        for (int i = 0; i < bullets.size(); i++) {
            if (bullets[i]->startTime != 0 && t - bullets[i]->startTime >= bulletTimeToLive || !bullets[i]->alive) {
                toDelete.insert(toDelete.begin(), i);
            } else {
                bullets[i]->step(state, t, dt);
            }
        }
        for (int i = 0; i < toDelete.size(); i++) {
			delete bullets[toDelete[i]];
            bullets.erase(bullets.begin() + toDelete[i]);
        }
    }

    void Gun::draw(GameState state) {
        for (int i = 0; i < bullets.size(); i++) {
            bullets[i]->draw(state);
        }
    }
}
