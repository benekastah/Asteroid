#pragma once

#include "glitter.hpp"

#include <vector>

#include "renderable.hpp"
#include "projectile.hpp"
#include "rigidbody.hpp"

namespace Asteroid {
    class Gun : Renderable {
    public:
        void initialize(int _maxBullets, float _bulletTimeToLive, float _bulletsPerSecond, float _cooldownRate);
        void fireBullet(double t, glm::vec2 vel, glm::vec2 pos);
        void step(GameState state, double t, double dt);
        void draw(GameState state);

    private:
        int maxBullets;
        float bulletTimeToLive;
        float bulletsPerSecond;
        float cooldownRate;
        std::vector<Projectile> bullets;
    };
}
