#pragma once
#include "glitter.hpp"
#include <GLFW/glfw3.h>

#include "glm/vec2.hpp"

#include "util.hpp"
#include "renderable.hpp"
#include "world.hpp"
#include "rigidbody.hpp"
#include "collider.hpp"

namespace Asteroid {
    class Crosshair : Renderable {
    public:
        GLuint shaderProgram;
        Rigidbody rb;
        Collider * coll;

        Crosshair();
        ~Crosshair();

        void step(double t, double dt);
        void draw();
        void onWorldChange(World world);

    private:
        GLuint vao, vbo;
        glm::vec4 color;
        bool onEnemy;
        GLint sizeRatio, colorUniform;

        void onCollide(const Collider other, const InteractionType t);
    };
}
