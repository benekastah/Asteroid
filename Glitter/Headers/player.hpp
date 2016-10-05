#pragma once
#include "glitter.hpp"
#include <GLFW/glfw3.h>

#include "glm/vec2.hpp"

#include "util.hpp"
#include "renderable.hpp"
#include "world.hpp"
#include "projectile.hpp"

namespace Asteroid {
    class Player : Renderable {
    public:
        GLuint shaderProgram;
        float mass;
        glm::vec2 force;
        glm::vec2 velocity;
        glm::vec2 pos;

        Player();
        ~Player();

        void step(GameState state, double t, double dt);

        void draw(Asteroid::GameState state);

    private:
        GLint direction, sizeRatio;
        GLuint vao, vbo;
        bool projectileActive;
        double projectileFiredAt = 0;
        double projectileTimeToLive = 1.2;
        Projectile * projectile;

        void onWorldChange(World world);
    };
}
