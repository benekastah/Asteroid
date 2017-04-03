#include "asteroid.hpp"

#include "game_state.hpp"
#include "explosion.hpp"

namespace Asteroid {
    Asteroid::Asteroid(float mMass, glm::vec2 mPos) {
        alive = true;
        mMass = maxf(minf(mMass, ASTEROID_MASS_MAX), ASTEROID_MASS_MIN);
        shaderProgram = createShaderProgram(
            shaderFile("asteroid.vert"), shaderFile("asteroid.geom"), shaderFile("asteroid.frag"));
        float r = findRadius(mMass, ASTEROID_DENSITY);
        rb = Rigidbody(mMass, mPos, r);
        coll = new Collider(&rb, ASTEROID);
        coll->addCollisionCallback(std::bind(&Asteroid::onCollision, this, std::placeholders::_1));
        coll->enable();

        direction = glGetUniformLocation(shaderProgram, "direction");
        sizeRatio = glGetUniformLocation(shaderProgram, "sizeRatio");
        radius = glGetUniformLocation(shaderProgram, "radius");
        rotation = glGetUniformLocation(shaderProgram, "rotation");
        color = glGetUniformLocation(shaderProgram, "color");
        warp = glGetUniformLocation(shaderProgram, "warp");

        glUseProgram(shaderProgram);
        glUniform1f(rotation, 1);
        glUniform1f(radius, World::getInstance().worldSizeToViewSize(r));
        glUniform4f(color, 1, 1, 1, 1);

        float warpVals[14] = {};
        size_t warpValsSize = sizeof(warpVals) / sizeof(warpVals[0]);
        for (int i = 0; i < warpValsSize; i++) {
            warpVals[i] = roundf(randfBtwn(-1, 1));
        }
        glUniform1fv(warp, warpValsSize, warpVals);

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        // Specify layout of point data
        GLint posAttrib = glGetAttribLocation(shaderProgram, "pos");
        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

        onWorldChange(World::getInstance());
        World::getInstance().addChangeCallback(std::bind(&Asteroid::onWorldChange, this, std::placeholders::_1));
    }

    Asteroid::~Asteroid() {
        glDeleteProgram(shaderProgram);
        delete coll;
    }

    void Asteroid::onCollision(const Collider other) {
        glUseProgram(shaderProgram);
        if (alive && other.type == PROJECTILE) {
            alive = false;
            GameState::getInstance()->explosions.push_back(new Explosion(rb.pos, glm::vec4(76/255., 60/255., 50/255., 0.1), rb.mass * 2/3, ASTEROID_DENSITY, .5));
        }
    }

    void Asteroid::step(double t, double dt) {
        if (!alive) {
            return;
        }
        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        rb.step(t, dt);
        float dir = atan2f(rb.velocity.y, rb.velocity.x);
        glUniform1f(direction, dir);
    }

    void Asteroid::draw() {
        if (!alive) {
            return;
        }
        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        if (rb.inBounds) {
            auto asteroidPos = World::getInstance().getPositionBuffer(rb.pos);
            glBufferData(GL_ARRAY_BUFFER, sizeof(asteroidPos.buf), &asteroidPos.buf, GL_STREAM_DRAW);
            glDrawArrays(GL_POINTS, 0, 9);
        } else {
            auto pos = World::getInstance().worldCoordToViewCoord(rb.pos);
            float asteroidPos[] = { pos.x, pos.y };
            glBufferData(GL_ARRAY_BUFFER, sizeof(asteroidPos), &asteroidPos, GL_STREAM_DRAW);
            glDrawArrays(GL_POINTS, 0, 1);
        }
    }

    void Asteroid::onWorldChange(World world) {
        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glUniform2f(sizeRatio, world.getGlRatio().x, world.getGlRatio().y);
    }
}
