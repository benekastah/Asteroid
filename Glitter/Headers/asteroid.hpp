#pragma once
#include "glitter.hpp"
#include <GLFW/glfw3.h>

#include "glm/vec2.hpp"

#include "util.hpp"
#include "renderable.hpp"
#include "rigidbody.hpp"
#include "collider.hpp"
#include "world.hpp"

#define ASTEROID_MASS_MIN 3000
#define ASTEROID_MASS_MAX 30000

namespace Asteroid {
    class GameState;

    class Asteroid : Renderable {
    public:
        GLuint shaderProgram;
        Rigidbody rb;
        Collider * coll;
        bool alive;

        Asteroid(float mass, glm::vec2 pos);
        ~Asteroid();

        void onCollision(Collider coll);

        void step(GameState state, double t, double dt);
        void draw(GameState state);

    private:
        GLint direction, sizeRatio, radius, rotation, color;
        GLuint vao, vbo;
        bool collided;
        void onWorldChange(World world);
    };
}
