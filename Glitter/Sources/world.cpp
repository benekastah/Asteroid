#include "world.hpp"

void windowSizeCallback(GLFWwindow* window, int width, int height) {
    Asteroid::World world = Asteroid::World::getInstance();
    world.setWindowSize(width, height);
}

namespace Asteroid {

    World instance;

    glm::vec2 worldTransforms[9];

    World World::getInstance() {
        return instance;
    }

    void World::setInstance(GLFWwindow * window) {
        World world = World();
        glfwSetWindowSizeCallback(window, windowSizeCallback);
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        world.setWindowSize(width, height);
        world.window = window;
        instance = world;

        worldTransforms[0] = glm::vec2(-WORLD_WIDTH, WORLD_HEIGHT);
        worldTransforms[1] = glm::vec2(0, WORLD_HEIGHT);
        worldTransforms[2] = glm::vec2(WORLD_WIDTH, WORLD_HEIGHT);
        worldTransforms[3] = glm::vec2(-WORLD_WIDTH, 0);
        worldTransforms[4] = glm::vec2(0, 0);
        worldTransforms[5] = glm::vec2(WORLD_WIDTH, 0);
        worldTransforms[6] = glm::vec2(-WORLD_WIDTH, -WORLD_HEIGHT);
        worldTransforms[7] = glm::vec2(0, -WORLD_HEIGHT);
        worldTransforms[8] = glm::vec2(WORLD_WIDTH, -WORLD_HEIGHT);
    }

    void World::setWindowSize(int width, int height) {
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

    glm::mat2 World::glBounds() {
        auto glMin = glm::vec2(-1, -1) * glRatio;
        auto glMax = glm::vec2(1, 1) * glRatio;
        return glm::mat2(glMin, glMax);
    }

    glm::vec2 World::worldCoordToViewCoord(glm::vec2 coord) {
        auto bounds = glBounds();
        auto glMin = bounds[0];
        auto glMax = bounds[1];
        return glm::vec2(
            (glMax.x - glMin.x) / WORLD_WIDTH * coord.x + glMin.x,
            (glMax.y - glMin.y) / WORLD_HEIGHT * coord.y + glMin.y
        );
    }

    glm::vec2 World::getMousePos() {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        y = winHeight - y;
        return glm::vec2(scale(0, winWidth, 0, WORLD_WIDTH, x), scale(0, winHeight, 0, WORLD_HEIGHT, y));
    }

    float World::worldSizeToViewSize(float size) {
        return (2.0f / WORLD_WIDTH) * size;
    }

    glm::vec2 World::getGlRatio() {
        return glRatio;
    }

    void World::addChangeCallback(std::function<void(World)> cb) {
        changeCallbacks.push_back(cb);
    }

    struct positionBuffer World::getPositionBuffer(glm::vec2 pos) {
        struct positionBuffer buf;
        for (int i = 0; i < 9; i++) {
            auto wPos = worldCoordToViewCoord(pos + worldTransforms[i]);
            buf.buf[i * 2] = wPos.x;
            buf.buf[(i * 2) + 1] = wPos.y;
        }
        return buf;
    }

    glm::vec2 World::size() {
        return glm::vec2(WORLD_WIDTH, WORLD_HEIGHT);
    }

    glm::vec2 World::center() {
        return size() / toVec2(2);
    }
}
