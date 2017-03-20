#pragma once
#include "glitter.hpp"
#include <GLFW/glfw3.h>

#include "glm/vec2.hpp"

#include "util.hpp"


namespace Asteroid {
    class GameState;

    class GameObject {
    public:
        virtual void GameObject::step(GameState state, double t, double dt) = 0;
    };
}
