#include "gun.hpp"

namespace Asteroid {

    Gun::Gun() {
        bulletMass = 25;
        bulletForce = 250000;
        maxBullets = 5;
        bulletTimeToLive = 0.6;
        bulletsPerSecond = 2.5;
        cooldownRate = 0;
    }

    Gun::~Gun() {
        clearBullets();
    }

    void Gun::clearBullets() {
        for (auto bullet : bullets) {
            delete bullet;
        }
        bullets.clear();
    }

    bool Gun::fireBullet(double t, glm::vec2 vel, glm::vec2 pos, glm::vec2 aim) {
        static double lastFiredBulletAt = 0;
        static double maxedOutAt = 0;
        if (t - lastFiredBulletAt < 1 / bulletsPerSecond) {
            return false;
        }
        if ((unsigned int) bullets.size() >= maxBullets) {
            maxedOutAt = t;
            return false;
        }
        if (t - maxedOutAt < cooldownRate) {
            return false;
        }
        lastFiredBulletAt = t;
        auto bullet = new Projectile(bulletMass, bulletForce, pos, vel, aim);
        bullets.push_back(bullet);
        return true;
    }

    void Gun::step(double t, double dt) {
        std::vector<int> toDelete;
        for (unsigned long i = 0; i < bullets.size(); i++) {
            if ((bullets[i]->startTime != 0 && t - bullets[i]->startTime >= bulletTimeToLive) || !bullets[i]->alive) {
                toDelete.insert(toDelete.begin(), i);
            } else {
                bullets[i]->step(t, dt);
            }
        }
        for (unsigned long i = 0; i < toDelete.size(); i++) {
            delete bullets[toDelete[i]];
            bullets.erase(bullets.begin() + toDelete[i]);
        }
    }

    void Gun::draw() {
        for (unsigned long i = 0; i < bullets.size(); i++) {
            bullets[i]->draw();
        }
    }
}
