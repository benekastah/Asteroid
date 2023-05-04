#pragma once

#include "glm/vec2.hpp"

#include "renderable.hpp"
#include "world.hpp"
#include "rigidbody.hpp"

namespace Asteroid {

    class Explosion : Renderable {
    public:
        GLuint shaderProgram;
        glm::vec2 pos;
        double startTime;
        double timeToLive;
        float mass;
        float density;
        float radius;
        glm::vec4 color;
        bool alive;

        Explosion(glm::vec2 mPos, glm::vec4 mColor, float mMass, float mDensity, double mTimeToLive);
        virtual ~Explosion();

        void step(double t, double dt);
        void draw();

    private:
        GLint radiusUniform, colorUniform, sizeRatio;
        GLuint vao, vbo, tex;
        std::vector<Rigidbody *> balls;

        void onWorldChange(World world);
    };
}
