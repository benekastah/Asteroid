#pragma once
#include "glitter.hpp"

#include <GLFW/glfw3.h>

#include "player.hpp"
#include "sidebar.hpp"
#include "asteroid.hpp"

namespace Asteroid {
    class GameState {
    public:
        GLFWwindow * window;
        Player * player;
        Sidebar * sidebar;
		std::vector<Asteroid *> asteroids;

        GameState(GLFWwindow * window);
        ~GameState();
    };
}
