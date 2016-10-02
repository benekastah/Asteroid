#pragma once
#include "glitter.hpp"
#include <GLFW/glfw3.h>

#include "glm/vec2.hpp"

#include "util.hpp"
#include "renderable.hpp"
#include "world.hpp"

namespace Asteroid {
    class Sidebar : Renderable {
    public:
        GLuint shaderProgram;

        Sidebar();
        ~Sidebar();

        void step(GameState state, double dt, double t);
        void draw(GameState state);
        void onWorldChange(World world);

    private:
        GLuint vao;
        GLuint vbo;
        GLuint ebo;
    };
}
