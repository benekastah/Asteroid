#include "gun.hpp"

#include "game_state.hpp"

namespace Asteroid {
    void Gun::initialize(int _maxBullets, float _bulletTimeToLive, float _bulletsPerSecond, float _cooldownRate) {
        maxBullets = _maxBullets;
        bulletTimeToLive = _bulletTimeToLive;
        bulletsPerSecond = _bulletsPerSecond;
        cooldownRate = _cooldownRate;
    }

    void Gun::fireBullet(double t, glm::vec2 vel, glm::vec2 pos) {
        static double lastFiredBulletAt = 0;
        static double maxedOutAt = 0;
        if (t - lastFiredBulletAt < 1 / bulletsPerSecond) {
            return;
        }
        if (bullets.size() >= maxBullets) {
            maxedOutAt = t;
            return;
        }
        if (t - maxedOutAt < cooldownRate) {
            return;
        }
        lastFiredBulletAt = t;
        auto bullet = new Projectile();
        bullet->initialize(pos, vel);
        bullets.push_back(bullet);
    }

    void Gun::step(GameState state, double t, double dt) {
        std::vector<int> toDelete;
        for (int i = 0; i < bullets.size(); i++) {
            if (bullets[i]->startTime != 0 && t - bullets[i]->startTime >= bulletTimeToLive || !bullets[i]->alive) {
				delete bullets[i];
                toDelete.insert(toDelete.begin(), i);
            } else {
                bullets[i]->step(state, t, dt);
            }
        }
        for (int i = 0; i < toDelete.size(); i++) {
            bullets.erase(bullets.begin() + toDelete[i]);
        }
    }

    void Gun::draw(GameState state) {
        for (int i = 0; i < bullets.size(); i++) {
            bullets[i]->draw(state);
        }
    }
}
