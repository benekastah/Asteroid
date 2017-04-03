#pragma once

#include "glitter.hpp"

#include <vector>

#include "renderable.hpp"
#include "projectile.hpp"
#include "rigidbody.hpp"

namespace Asteroid {
    class Gun : Renderable {
    public:
        int maxBullets;
        float bulletTimeToLive;
        float bulletsPerSecond;
        float cooldownRate;
        float bulletMass;
        float bulletForce;

        Gun();
        ~Gun();
        bool fireBullet(double t, glm::vec2 vel, glm::vec2 pos, glm::vec2 aim);
        void step(double t, double dt);
        void draw();
        void clearBullets();

    private:
        std::vector<Projectile *> bullets;
    };
}
