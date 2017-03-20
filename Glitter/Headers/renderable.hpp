#pragma once
#include "glitter.hpp"
#include <GLFW/glfw3.h>

#include "glm/vec2.hpp"

#include "util.hpp"
#include "game_object.hpp"


namespace Asteroid {
    class GameState;

    class Renderable : GameObject {
    public:
        virtual void Renderable::draw(GameState state) = 0;
    };
}
