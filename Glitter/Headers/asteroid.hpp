#pragma once
#include "glitter.hpp"
#include <GLFW/glfw3.h>

#include "glm/vec2.hpp"

#include "util.hpp"
#include "renderable.hpp"
#include "world.hpp"

namespace Asteroid {
    class GameState;

    class Asteroid : Renderable {
    public:
        GLuint shaderProgram;
        float mass;
        glm::vec2 velocity;
        glm::vec2 pos;

        Asteroid(float mass, glm::vec2 velocity, glm::vec2 pos);
        ~Asteroid();

        void step(GameState state, double t, double dt);
        void draw(GameState state);

    private:
        GLint sizeRatio;
        GLuint vao, vbo;
        void onWorldChange(World world);
    };
}
