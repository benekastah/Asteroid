#include "game_state.hpp"

namespace Asteroid {
    GameState::GameState(GLFWwindow * window) {
        this->window = window;
        player = new Player();
        sidebar = new Sidebar();
    }

    GameState::~GameState() {}
}
