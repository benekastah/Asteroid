#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <vector>

#define PI 3.141592653589793

char* readFile(const char* fname);

float scale(float min1, float max1, float min2, float max2, float val);
float randfBtwn(float min, float max);

float minf(float a, float b);
float maxf(float a, float b);

void print_gl_errors();

#define shaderFile(name) (PROJECT_SOURCE_DIR "/Glitter/Shaders/" name)
GLuint createShader(GLenum type, const char * fname);
#define createShaderProgram(...) (_createShaderProgram(__VA_ARGS__, nullptr))
GLuint _createShaderProgram(const char * name, ...);
void deleteShaderProgram(GLuint shaderProgram);
