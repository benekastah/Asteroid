#pragma once
#include "glitter.hpp"
#include <GLFW/glfw3.h>

#include "glm/vec2.hpp"

#include "util.hpp"
#include "renderable.hpp"
#include "world.hpp"

namespace Asteroid {
    class Projectile : Renderable {
    public:
        GLuint shaderProgram;
        float speed;
        float angle;
        glm::vec2 pos;

        Projectile(float pAngle, float pSpeed, glm::vec2 pPos);
        ~Projectile();

        void step(GameState state, double t, double dt);
        void draw(GameState state);

        void onWorldChange(World world);

    private:
        GLuint vao, vbo;
        GLuint sizeRatio;
    };
}
