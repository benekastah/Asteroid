#include "game_state.hpp"

namespace Asteroid {
    GameState::GameState(GLFWwindow * window) {
        this->window = window;
        player = new Player();
		auto safeSpace = Collider(&player->rb, 15, PLAYER);
        sidebar = new Sidebar();
		for (int i = 0; i < 6; i++) {
			auto asteroid = new Asteroid(randfBtwn(ASTEROID_MASS_MIN, ASTEROID_MASS_MAX), World::getInstance().randPos());
			while (asteroid->coll->intersects(safeSpace)) {
				asteroid->rb.pos = World::getInstance().randPos();
			}
			auto force = glm::vec2(
				randfBtwn(asteroid->rb.mass * -2000, asteroid->rb.mass * 2000),
				randfBtwn(asteroid->rb.mass * -2000, asteroid->rb.mass * 2000));
			asteroid->rb.applyForce(force);
			asteroids.push_back(asteroid);
		}
		//auto asteroid1 = new Asteroid(ASTEROID_MASS_MAX / 2, glm::vec2(99, 50));
		//asteroids.push_back(asteroid1);
		//auto asteroid2 = new Asteroid(ASTEROID_MASS_MAX / 2, glm::vec2(1, 45));
		//asteroids.push_back(asteroid2);
    }

	void GameState::step(double t, double dt) {
		player->step(*this, t, dt);

		std::vector<int> toDelete;
		for (int i = 0; i < asteroids.size(); i++) {
			auto asteroid = asteroids[i];
			if (asteroid->alive) {
				asteroid->step(*this, t, dt);
			} else {
				float mass = asteroid->rb.mass / 2;
				if (mass >= ASTEROID_MASS_MIN) {
					for (int i = 0; i < 2; i++) {
						auto ast = new Asteroid(mass, asteroid->rb.pos);
						ast->rb.applyVelocity(asteroid->rb.velocity);
						asteroids.push_back(ast);
					}
				}
				delete asteroid;
				toDelete.insert(toDelete.begin(), i);
			}
		}
		for each (int i in toDelete) {
			asteroids.erase(asteroids.begin() + i);
		}
	}

	void GameState::draw() {
		player->draw(*this);
		for each (auto asteroid in asteroids) {
			asteroid->draw(*this);
		}
		sidebar->draw(*this);
	}
}
