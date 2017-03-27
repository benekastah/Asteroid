#pragma once
#include "glitter.hpp"
#include <GLFW/glfw3.h>

#include "glm/vec2.hpp"

#include "util.hpp"
#include "renderable.hpp"
#include "world.hpp"
#include "collider.hpp"
#include "rigidbody.hpp"

namespace Asteroid {
    class Projectile : Renderable {
    public:
        float startTime;
		Rigidbody rb;
		Collider * coll;
		bool alive;

		~Projectile();
        Projectile(float mass, float force, glm::vec2 pPos, glm::vec2 vel);

        void step(GameState state, double t, double dt);
        void draw(GameState state);

        void onWorldChange(World world);

    private:
        GLuint shaderProgram;
        GLuint vao, vbo;
        GLuint sizeRatio, radius;

		void onCollide(const Collider coll);
    };
}