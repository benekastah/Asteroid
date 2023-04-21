#include <functional>

#include "crosshair.hpp"
#include "world.hpp"
#include "game_state.hpp"

namespace Asteroid {
    Crosshair::Crosshair() {
        shaderProgram = createShaderProgram(
            shaderFile("crosshair.vert"), shaderFile("crosshair.geom"), shaderFile("crosshair.frag"));
        sizeRatio = glGetUniformLocation(shaderProgram, "sizeRatio");
        colorUniform = glGetUniformLocation(shaderProgram, "color");

        rb = Rigidbody(1, glm::vec2(0, 0), 1.3);
        coll = new Collider(&rb, CROSSHAIR);
        coll->enable();
        coll->addCollisionCallback([&](auto other, auto t) {
            onCollide(other, t);
        });

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        // Specify layout of point data
        GLint posAttrib = glGetAttribLocation(shaderProgram, "pos");
        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glUseProgram(shaderProgram);
        glUniform1f(glGetUniformLocation(shaderProgram, "radius"), World::getInstance().worldSizeToViewSize(rb.radius));

        auto world = World::getInstance();
        onWorldChange(world);
        world.addChangeCallback([&](auto world) {
            onWorldChange(world);
        });
    }

    Crosshair::~Crosshair() {
        glDeleteProgram(shaderProgram);
    }

    void Crosshair::step(double t, double dt) {
        rb.applyPos(World::getInstance().getMousePos());
        rb.step(t, dt);
        if (onEnemy) {
            color = glm::vec4(1, 0, 0, 1);
            onEnemy = false;
        } else {
            color = glm::vec4(.8, .8, .8, 1);
        }
    }

    void Crosshair::draw() {
        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glUniform4f(colorUniform, color.r, color.g, color.b, color.a);
        glm::vec2 viewPos = World::getInstance().worldCoordToViewCoord(rb.pos);
        glBufferData(GL_ARRAY_BUFFER, sizeof(viewPos), &viewPos, GL_STREAM_DRAW);
        glDrawArrays(GL_POINTS, 0, 1);
    }

    void Crosshair::onWorldChange(World world) {
        glUseProgram(shaderProgram);
        glUniform2f(sizeRatio, world.getGlRatio().x, world.getGlRatio().y);
    }

    void Crosshair::onCollide(const Collider other, const InteractionType t) {
        if (other.type == ASTEROID) {
            onEnemy = true;
        }
    }
}
