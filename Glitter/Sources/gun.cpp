#include "gun.hpp"

#include "game_state.hpp"

namespace Asteroid {
    void Gun::initialize(int _maxBullets, float _bulletTimeToLive, float _bulletsPerSecond, float _cooldownRate) {
        maxBullets = _maxBullets;
        bulletTimeToLive = _bulletTimeToLive;
        bulletsPerSecond = _bulletsPerSecond;
        cooldownRate = _cooldownRate;
    }

    void Gun::fireBullet(double t, float angle, float speed, glm::vec2 pos) {
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
        Projectile bullet;
        bullet.initialize(angle, speed, pos);
        bullets.push_back(bullet);
    }

    void Gun::step(GameState state, double t, double dt) {
        std::vector<int> toDelete;
        for (int i = 0; i < bullets.size(); i++) {
            if (bullets[i].startTime != 0 && t - bullets[i].startTime >= bulletTimeToLive) {
                toDelete.insert(toDelete.begin(), i);
            } else {
                bullets[i].step(state, t, dt);
            }
        }
        for (int i = 0; i < toDelete.size(); i++) {
            bullets.erase(bullets.begin() + toDelete[i]);
        }
    }

    void Gun::draw(GameState state) {
        for (int i = 0; i < bullets.size(); i++) {
            bullets[i].draw(state);
        }
    }
}
