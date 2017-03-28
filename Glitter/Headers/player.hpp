#pragma once
#include "glitter.hpp"
#include <GLFW/glfw3.h>

#include "glm/vec2.hpp"

#include "util.hpp"
#include "renderable.hpp"
#include "world.hpp"
#include "gun.hpp"
#include "rigidbody.hpp"
#include "collider.hpp"

namespace Asteroid {
    class GameState;

    class Player : Renderable {
    public:
        GLuint shaderProgram;
        Rigidbody rb;
        Collider * coll;
        bool alive;
        Gun gun;

        Player();
        ~Player();

        void step(GameState state, double t, double dt);
        void draw(GameState state);

    private:
        GLint direction, radius, sizeRatio;
        GLuint vao, vbo;

        void onWorldChange(World world);
        void onCollide(const Collider coll);
    };
}
