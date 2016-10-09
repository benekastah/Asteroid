#include "player.hpp"

#include <math.h>

#include "world.hpp"
#include "game_state.hpp"

Asteroid::Player::Player() {
    shaderProgram = createShaderProgram(
        shaderFile("player.vert"), shaderFile("player.geo"), shaderFile("player.frag"));
    mass = 5000;
    force = glm::vec2(0, 0);
    velocity = glm::vec2(0, 0);
    pos = glm::vec2(50, 50);
    direction = glGetUniformLocation(shaderProgram, "direction");
    sizeRatio = glGetUniformLocation(shaderProgram, "sizeRatio");

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Specify layout of point data
    GLint posAttrib = glGetAttribLocation(shaderProgram, "pos");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

    onWorldChange(World::getInstance());
    World::getInstance().addChangeCallback(std::bind(&Asteroid::Player::onWorldChange, this, std::placeholders::_1));

    gun.initialize(5, 1.2, 3, 0.5);
}

Asteroid::Player::~Player() {
    glDeleteProgram(shaderProgram);
}

void Asteroid::Player::step(GameState state, double t, double dt) {
    glUseProgram(shaderProgram);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    const float maxForce = 50000 * 50;
    const float forcePerStep = maxForce / 10;
    auto window = state.window;

    force.y = 0;
    force.x = 0;
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

    applyForce(force, dt, mass, &velocity, &pos);
    const float maxVelocity = 100;
    if (velocity.x < -maxVelocity) { velocity.x = -maxVelocity; }
    if (velocity.x > maxVelocity) { velocity.x = maxVelocity; }
    if (velocity.y < -maxVelocity) { velocity.y = -maxVelocity; }
    if (velocity.y > maxVelocity) { velocity.y = maxVelocity; }

    auto world = World::getInstance();
    pos = world.wrapWorldCoord(pos);

    float dir = atan2f(velocity.y, velocity.x);
    glUniform1f(direction, dir);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        float speed = sqrtf(powf(velocity.x, 2) + powf(velocity.y, 2));
        gun.fireBullet(t, dir, speed + 50, pos);
    }

    gun.step(state, t, dt);
}

void Asteroid::Player::draw(Asteroid::GameState state) {
    glUseProgram(shaderProgram);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    World world = World::getInstance();
    glm::vec2 world1 = world.worldCoordToViewCoord(glm::vec2(pos.x - WORLD_WIDTH, pos.y + WORLD_HEIGHT));
    glm::vec2 world2 = world.worldCoordToViewCoord(glm::vec2(pos.x, pos.y + WORLD_HEIGHT));
    glm::vec2 world3 = world.worldCoordToViewCoord(glm::vec2(pos.x + WORLD_WIDTH, pos.y + WORLD_HEIGHT));
    glm::vec2 world4 = world.worldCoordToViewCoord(glm::vec2(pos.x - WORLD_WIDTH, pos.y));
    glm::vec2 world5 = world.worldCoordToViewCoord(glm::vec2(pos.x, pos.y));
    glm::vec2 world6 = world.worldCoordToViewCoord(glm::vec2(pos.x + WORLD_WIDTH, pos.y));
    glm::vec2 world7 = world.worldCoordToViewCoord(glm::vec2(pos.x - WORLD_WIDTH, pos.y - WORLD_HEIGHT));
    glm::vec2 world8 = world.worldCoordToViewCoord(glm::vec2(pos.x, pos.y - WORLD_HEIGHT));
    glm::vec2 world9 = world.worldCoordToViewCoord(glm::vec2(pos.x + WORLD_WIDTH, pos.y - WORLD_HEIGHT));
    float playerPos[] = {
        world1.x, world1.y,
        world2.x, world2.y,
        world3.x, world3.y,
        world4.x, world4.y,
        world5.x, world5.y,
        world6.x, world6.y,
        world7.x, world7.y,
        world8.x, world8.y,
        world9.x, world9.y
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(playerPos), &playerPos, GL_STREAM_DRAW);
    glDrawArrays(GL_POINTS, 0, 9);

    gun.draw(state);
}

void Asteroid::Player::onWorldChange(World world) {
    glUseProgram(shaderProgram);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glUniform2f(sizeRatio, world.getGlRatio().x, world.getGlRatio().y);
}
