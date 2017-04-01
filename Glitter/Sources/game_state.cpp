#include "game_state.hpp"

namespace Asteroid {

    static const float SPAWN_RADIUS = sqrtf(powf(WORLD_WIDTH / 2, 2) + powf(WORLD_HEIGHT / 2, 2));

    GameState::GameState(GLFWwindow * window) {
        resetAt = 0;
        nextLevelAt = 0;
        this->window = window;
        player = new Player();
        sidebar = new Sidebar();
        level = 2;
        asteroidsLoaded = 0;
        loadLevel();
    }

    void GameState::clearAsteroids() {
        for each (auto asteroid in asteroids) {
            delete asteroid;
        }
        asteroids.clear();
    }

    void pushInBounds(Asteroid * asteroid) {
        auto center = World::getInstance().center();
        auto dest = pointOnCircle(center, SPAWN_RADIUS / 2, randfBtwn(0, 2 * PI));
        asteroid->rb.applyVelocity(glm::normalize(dest - asteroid->rb.pos) * toVec2(randfBtwn(10, 35)));
    }

    void GameState::addAsteroid(double t) {
        auto center = World::getInstance().center();
        float mass = randfBtwn(ASTEROID_MASS_MAX / 3, ASTEROID_MASS_MAX);
        auto pos = pointOnCircle(center, SPAWN_RADIUS, randfBtwn(0, 2 * PI));
        auto asteroid = new Asteroid(mass, pos);
        pushInBounds(asteroid);
        asteroids.push_back(asteroid);
        asteroidsLoaded++;
        loadNextAsteroidAt = t + ((double) randfBtwn(0, powf(asteroids.size(), .5)) * 2);
    }

    void GameState::loadLevel() {
        clearAsteroids();
        asteroidsLoaded = 0;
        loadNextAsteroidAt = 0;
    }

    void GameState::step(double t, double dt) {
        player->step(*this, t, dt);

        if (resetAt == 0 && !player->alive) {
            resetAt = t + PAUSE_BETWEEN_LEVELS;
        } else if (nextLevelAt == 0 && asteroidsLoaded >= level && asteroids.size() == 0) {
            nextLevelAt = t + PAUSE_BETWEEN_LEVELS;
        }

        if (resetAt != 0 && resetAt < t) {
            resetAt = 0;
            nextLevelAt = 0;
            delete player;
            player = new Player();
            loadLevel();
        } else if (nextLevelAt != 0 && nextLevelAt < t) {
            resetAt = 0;
            nextLevelAt = 0;
            player->gun.clearBullets();
            level++;
            loadLevel();
        }

        if (asteroidsLoaded < level && (loadNextAsteroidAt < t || asteroids.size() == 0)) {
            addAsteroid(t);
        }

        std::vector<int> toDelete;
        for (int i = 0; i < asteroids.size(); i++) {
            auto asteroid = asteroids[i];
            if (asteroid->alive) {
                if (!asteroid->rb.inBounds && glm::distance(asteroid->rb.pos, World::getInstance().center()) > SPAWN_RADIUS) {
                    // Catch asteroids that are moving further from the center rather than closer
                    pushInBounds(asteroid);
                }
                asteroid->step(*this, t, dt);
            } else {
                float mass = asteroid->rb.mass / 3;
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
