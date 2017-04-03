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

#define PLAYER_DENSITY 608
#define PLAYER_MASS 5000

// The velocity at which the player can be considered stopped
#define PLAYER_STOP_VELOCITY 0.3

namespace Asteroid {

    class Player : Renderable {
    public:
        GLuint shaderProgram;
        Rigidbody rb;
        Collider * coll;
        bool alive;
        Gun gun;
        glm::vec2 aim;

        Player();
        ~Player();

        void step(double t, double dt);
        void draw();

    private:
        GLint directionUniform, radius, sizeRatio;
        GLuint vao, vbo;

        void onWorldChange(World world);
        void onCollide(const Collider coll, const InteractionType t);
    };
}
