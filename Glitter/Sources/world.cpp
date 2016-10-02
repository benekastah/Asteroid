#include "world.hpp"

using namespace Asteroid;

void windowSizeCallback(GLFWwindow* window, int width, int height) {
    World world = World::getInstance();
    world.setWindowSize(width, height);
}

World instance;

World Asteroid::World::getInstance() {
    return instance;
}

void Asteroid::World::setInstance(GLFWwindow * window) {
    World world = World();
    glfwSetWindowSizeCallback(window, windowSizeCallback);
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    world.setWindowSize(width, height);
    instance = world;
}

void Asteroid::World::setWindowSize(int width, int height) {
    winWidth = width;
    winHeight = height;
    glRatio = glm::vec2(
        width > height ? (float) height / (float) width : 1,
        height > width ? (float) width / (float) height : 1
    );

    for each (std::function<void(World)> cb in changeCallbacks) {
        cb(*this);
    }
}

glm::mat2 Asteroid::World::glBounds() {
    auto glMin = glm::vec2(-1, -1) * glRatio;
    auto glMax = glm::vec2(1, 1) * glRatio;
    return glm::mat2(glMin, glMax);
}

glm::vec2 Asteroid::World::worldCoordToViewCoord(glm::vec2 coord) {
    auto bounds = glBounds();
    auto glMin = bounds[0];
    auto glMax = bounds[1];
    return glm::vec2(
        (glMax.x - glMin.x) / WORLD_WIDTH * coord.x + glMin.x,
        (glMax.y - glMin.y) / WORLD_HEIGHT * coord.y + glMin.y
    );
}

glm::vec2 Asteroid::World::wrapWorldCoord(glm::vec2 coord) {
    if (coord.x < 0) {
        coord.x += WORLD_WIDTH;
    } else if (coord.x > WORLD_WIDTH) {
        coord.x -= WORLD_WIDTH;
    }
    if (coord.y < 0) {
        coord.y += WORLD_HEIGHT;
    } else if (coord.y > WORLD_HEIGHT) {
        coord.y -= WORLD_HEIGHT;
    }
    return coord;
}

glm::vec2 Asteroid::World::getGlRatio() {
    return glRatio;
}

void Asteroid::World::addChangeCallback(std::function<void(World)> cb) {
    changeCallbacks.push_back(cb);
}
