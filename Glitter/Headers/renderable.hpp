#pragma once
#include "glitter.hpp"
#include <GLFW/glfw3.h>

#include "glm/vec2.hpp"

#include "util.hpp"


namespace Asteroid {
    class GameState;

    class Renderable {
    public:
        virtual void Renderable::step(GameState state, double t, double dt) = 0;
        virtual void Renderable::draw(GameState state) = 0;
    };
}
