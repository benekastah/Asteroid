#include "player.hpp"

#include <math.h>

#include "game_state.hpp"

namespace Asteroid {
    Player::Player() {
        alive = true;
        shaderProgram = createShaderProgram(
            shaderFile("player.vert"), shaderFile("player.geo"), shaderFile("player.frag"));
        float r = findRadius(PLAYER_MASS, PLAYER_DENSITY);
        rb = Rigidbody(PLAYER_MASS, World::getInstance().center(), r);
        rb.maxVelocity = 100;
        coll = new Collider(&rb, PLAYER);
        coll->addCollisionCallback(std::bind(&Player::onCollide, this, std::placeholders::_1));
        coll->enable();
        direction = glGetUniformLocation(shaderProgram, "direction");
        radius = glGetUniformLocation(shaderProgram, "radius");
        sizeRatio = glGetUniformLocation(shaderProgram, "sizeRatio");

        glUseProgram(shaderProgram);
        glUniform1f(radius, World::getInstance().worldSizeToViewSize(r));

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        // Specify layout of point data
        GLint posAttrib = glGetAttribLocation(shaderProgram, "pos");
        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

        onWorldChange(World::getInstance());
        World::getInstance().addChangeCallback(std::bind(&Player::onWorldChange, this, std::placeholders::_1));
    }

    Player::~Player() {
        glDeleteProgram(shaderProgram);
        delete coll;
    }

    void Player::step(double t, double dt) {
        if (!alive) {
            coll->disable();
            return;
        }
        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        const float maxForce = 50000 * 50;
        const float forcePerStep = maxForce / 10;
        auto window = GameState::getInstance()->window;

        auto force = glm::vec2(0, 0);
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            force.y += forcePerStep;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            force.y -= forcePerStep;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            force.x -= forcePerStep;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            force.x += forcePerStep;
        }
        force.x = minf(maxForce, maxf(-maxForce, force.x));
        force.y = minf(maxForce, maxf(-maxForce, force.y));

        rb.applyForce(force);
        rb.step(t, dt);
        float dir = atan2f(rb.velocity.y, rb.velocity.x);
        glUniform1f(direction, dir);

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            gun.fireBullet(t, rb.velocity, rb.pos);
        }

        gun.step(t, dt);
    }

    void Player::draw() {
        if (!alive) {
            return;
        }
        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        World world = World::getInstance();
        auto playerPos = world.getPositionBuffer(rb.pos);
        glBufferData(GL_ARRAY_BUFFER, sizeof(playerPos), &playerPos.buf, GL_STREAM_DRAW);
        glDrawArrays(GL_POINTS, 0, 9);

        gun.draw();
    }

    void Player::onWorldChange(World world) {
        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glUniform2f(sizeRatio, world.getGlRatio().x, world.getGlRatio().y);
    }

    void Player::onCollide(const Collider other) {
        alive = false;
    }
}
