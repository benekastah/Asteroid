#pragma once
#include "glitter.hpp"
#include <GLFW/glfw3.h>

#include "glm/vec2.hpp"

#include "util.hpp"


namespace Asteroid {

    class GameObject {
    public:
        virtual void step(double t, double dt) = 0;
    };
}
