#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <vector>

#define PI 3.141592653589793

char* readFile(const char* fname);

float scale(float min1, float max1, float min2, float max2, float val);
void _srand();
float randf();
float randfBtwn(float min, float max);

glm::vec2 toVec2(float x);
glm::vec2 randVec2();
glm::vec2 randVec2(float magnitude);
glm::vec2 randVec2(float min, float max);
glm::vec2 pointOnCircle(glm::vec2 pos, float r, float dir);
float distance(glm::vec2 a, glm::vec2 b);

float minf(float a, float b);
float maxf(float a, float b);

void print_gl_errors();

#define shaderFile(name) (PROJECT_SOURCE_DIR "/Glitter/Shaders/" name)
GLuint createShader(GLenum type, const char * fname);
#define createShaderProgram(...) (_createShaderProgram(__VA_ARGS__, nullptr))
GLuint _createShaderProgram(const char * name, ...);
void deleteShaderProgram(GLuint shaderProgram);
