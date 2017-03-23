// Local Headers
#include "glitter.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>

#include "game_state.hpp"
#include "util.hpp"
#include "world.hpp"
#include "collision_manager.hpp"

using namespace Asteroid;

void render(GameState state) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    state.player->draw(state);
	for each (auto asteroid in state.asteroids) {
		asteroid->draw(state);
	}
    state.sidebar->draw(state);

    // Flip Buffers and Draw
    glfwSwapBuffers(state.window);
    glfwPollEvents();
}

void updateState(GameState * state, double t, double dt) {
    if (glfwGetKey(state->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(state->window, true);
    }
	CollisionManager::getInstance().collide();
	if (state->player->alive) {
		state->player->step(*state, t, dt);
	}

	std::vector<int> toDelete;
	for (int i = 0; i < state->asteroids.size(); i++) {
		auto asteroid = state->asteroids[i];
		if (asteroid->alive) {
			asteroid->step(*state, t, dt);
		} else {
			delete asteroid;
			toDelete.insert(toDelete.begin(), i);
		}
	}
	for each (int i in toDelete) {
		state->asteroids.erase(state->asteroids.begin() + i);
	}
}

// http://gafferongames.com/game-physics/fix-your-timestep/
void gameLoop(GLFWwindow * window) {
    double t = 0.0;
    double dt = 0.01;
    double maxFrameTime = 0.25;

    double currentTime = glfwGetTime();
    double accumulator = 0.0;

    World::setInstance(window);
    GameState state(window);

    while (!glfwWindowShouldClose(window)) {
        double newTime = glfwGetTime();
        double frameTime = newTime - currentTime;
        if (frameTime > maxFrameTime) {
            frameTime = maxFrameTime;
        }
        currentTime = newTime;
        accumulator += frameTime;

        while (accumulator >= dt) {
            updateState(&state, t, dt);
            t += dt;
            accumulator -= dt;
        }

        render(state);
    }
}

int main(int argc, char * argv[]) {

    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    auto mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);

    // Check for Valid Context
    if (mWindow == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        return EXIT_FAILURE;
    }

    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow);
    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

    gameLoop(mWindow);

    glfwTerminate();
    return EXIT_SUCCESS;
}
