#pragma once
#include "glitter.hpp"
#include <GLFW/glfw3.h>

#include "glm/vec2.hpp"

#include "util.hpp"
#include "renderable.hpp"
#include "rigidbody.hpp"
#include "collider.hpp"
#include "world.hpp"

#define ASTEROID_MASS_MIN 10000
#define ASTEROID_MASS_MAX 1000000

// in kg/m^3
#define ASTEROID_DENSITY 1380

namespace Asteroid {

    class Asteroid : Renderable {
    public:
        GLuint shaderProgram;
        Rigidbody rb;
        Collider * coll;
        bool alive;

        Asteroid(float mass, glm::vec2 pos);
        ~Asteroid();

        void onCollision(Collider coll);

        void step(double t, double dt);
        void draw();

    private:
        GLint direction, sizeRatio, radius, rotation, color, warp;
        GLuint vao, vbo;
        void onWorldChange(World world);
    };
}
