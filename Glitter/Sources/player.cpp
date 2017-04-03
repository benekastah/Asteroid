#include "player.hpp"

#include <math.h>

#include "game_state.hpp"
#include "explosion.hpp"

namespace Asteroid {
    Player::Player() {
        alive = true;
        shaderProgram = createShaderProgram(
            shaderFile("player.vert"), shaderFile("player.geo"), shaderFile("player.frag"));
        float r = findRadius(PLAYER_MASS, PLAYER_DENSITY);
        rb = Rigidbody(PLAYER_MASS, World::getInstance().center(), r);
        rb.maxVelocity = 100;
        coll = new Collider(&rb, PLAYER);
        coll->addCollisionCallback(std::bind(&Player::onCollide, this, std::placeholders::_1, std::placeholders::_2));
        coll->enable();
        directionUniform = glGetUniformLocation(shaderProgram, "direction");
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

        auto mousePos = World::getInstance().getMousePos();
        aim = glm::normalize(mousePos - rb.pos);
        float dir = atan2f(aim.y, aim.x);
        glUniform1f(directionUniform, dir);

        const float forcePerStep = 750000 / 2;
        auto window = GameState::getInstance()->window;

        bool up = glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
        bool left = glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
        bool down = glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
        bool right = glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;

        glm::vec2 force;
        if (up || down || left || right) {
            force = glm::vec2(right - left, up - down);
            if (glm::length(force) != 0) {
                force = glm::normalize(force) * toVec2(forcePerStep);
            }
        } else if (glm::length(rb.velocity) != 0) {
            // Apply stopping force
            auto deltaVel = (rb.velocity - toVec2(PLAYER_STOP_VELOCITY) * glm::normalize(rb.velocity)) * toVec2(-1);
            auto stopForce = toVec2(rb.mass) * (deltaVel / (float) dt);
            if (glm::length(stopForce) != 0) {
                force = glm::normalize(stopForce) * toVec2(forcePerStep);
                if (glm::length(force) > glm::length(stopForce)) {
                    force = stopForce;
                }
            }
        }

        rb.applyForce(force);
        rb.step(t, dt);

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS || glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
            gun.fireBullet(t, rb.velocity, rb.pos, aim);
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

    void Player::onCollide(const Collider other, const InteractionType t) {
        if (t != COLLIDE) {
            return;
        }
        if (alive) {
            GameState::getInstance()->explosions.push_back(new Explosion(rb.pos, glm::vec4(1, 0.0f, 0, 0.6), PLAYER_MASS * 3, PLAYER_DENSITY, 0.75));
        }
        alive = false;
    }
}
