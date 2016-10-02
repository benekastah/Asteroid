#pragma once
#include "glitter.hpp"

#include <GLFW/glfw3.h>

#include "player.hpp"
#include "sidebar.hpp"

namespace Asteroid {
    class GameState {
    public:
        GLFWwindow * window;
        Player * player;
        Sidebar * sidebar;

        GameState(GLFWwindow * window);
        ~GameState();
    };
}
