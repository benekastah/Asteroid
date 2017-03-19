#pragma once
#include "glitter.hpp"
#include <GLFW/glfw3.h>

#include "glm/vec2.hpp"

#include "util.hpp"
#include "renderable.hpp"
#include "rigidbody.hpp"
#include "world.hpp"

#define ASTEROID_MASS_MIN 1000
#define ASTEROID_MASS_MAX 10000

namespace Asteroid {
    class GameState;

    class Asteroid : Renderable {
    public:
        GLuint shaderProgram;
		Rigidbody rb;

        Asteroid(float mass, glm::vec2 pos);
        ~Asteroid();

        void step(GameState state, double t, double dt);
        void draw(GameState state);

    private:
        GLint direction, sizeRatio, size, rotation;
        GLuint vao, vbo;
        void onWorldChange(World world);
    };
}
