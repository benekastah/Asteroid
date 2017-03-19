#pragma once

#include "glitter.hpp"
#include <GLFW/glfw3.h>
#include <vector>
#include <functional>

#include "glm/vec2.hpp"

#include "util.hpp"

#define WORLD_WIDTH 100
#define WORLD_HEIGHT 100

namespace Asteroid {
	struct positionBuffer {
		float buf[18];
	};

    class World {
    public:
        int winWidth;
        int winHeight;

        static World getInstance();
        static void setInstance(GLFWwindow * window);

        void setWindowSize(int width, int height);

        glm::mat2 glBounds();
        glm::vec2 worldCoordToViewCoord(glm::vec2 coord);
		float worldSizeToViewSize(float size);
        glm::vec2 wrapWorldCoord(glm::vec2 coord);
        glm::vec2 getGlRatio();
        void addChangeCallback(std::function<void(World)> cb);
		struct positionBuffer getPositionBuffer(glm::vec2 pos);
		glm::vec2 randPos();

    private:
        glm::vec2 glRatio;
        std::vector<std::function<void(World)>> changeCallbacks;
    };
}
