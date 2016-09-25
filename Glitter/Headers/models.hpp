#pragma once
#include <GLFW/glfw3.h>

typedef struct {
    GLFWwindow * window;
    GLuint playerShaderProgram;
} GameState;