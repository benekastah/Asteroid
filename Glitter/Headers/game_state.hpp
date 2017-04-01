#pragma once
#include "glitter.hpp"

#include <GLFW/glfw3.h>

#include "player.hpp"
#include "sidebar.hpp"
#include "asteroid.hpp"

#define PAUSE_BETWEEN_LEVELS 2

namespace Asteroid {

    class GameState {
    public:
        GLFWwindow * window;
        Player * player;
        Sidebar * sidebar;
        std::vector<Asteroid *> asteroids;
        unsigned int level;

        static GameState * getInstance();
        GameState(GLFWwindow * window);

        void step(double t, double dt);
        void draw();
        void loadLevel();
        void clearAsteroids();
        void addAsteroid(double t);

    private:
        double resetAt;
        double nextLevelAt;
        double loadNextAsteroidAt;
        unsigned int asteroidsLoaded;
    };
}
