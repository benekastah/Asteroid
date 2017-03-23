#include "game_state.hpp"

namespace Asteroid {
    GameState::GameState(GLFWwindow * window) {
        this->window = window;
        player = new Player();
        sidebar = new Sidebar();
		for (int i = 0; i < 6; i++) {
			auto asteroid = new Asteroid(randfBtwn(ASTEROID_MASS_MIN, ASTEROID_MASS_MAX), World::getInstance().randPos());
			auto force = glm::vec2(
				randfBtwn(asteroid->rb.mass * -2000, asteroid->rb.mass * 2000),
				randfBtwn(asteroid->rb.mass * -2000, asteroid->rb.mass * 2000));
			asteroid->rb.applyForce(force);
			asteroids.push_back(asteroid);
		}
    }

    GameState::~GameState() {}
}
