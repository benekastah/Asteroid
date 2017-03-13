#include "asteroid.hpp"

#include "game_state.hpp"

static GLuint asteroidShaderProgram;
static bool asteroidShaderProgramInit = false;

namespace Asteroid {
    Asteroid::Asteroid(float mMass, glm::vec2 mVelocity, glm::vec2 mPos) {
        if (!asteroidShaderProgramInit) {
            asteroidShaderProgram = createShaderProgram(
                shaderFile("asteroid.vert"), shaderFile("asteroid.geo"), shaderFile("asteroid.frag"));
            asteroidShaderProgramInit = true;
        }
        shaderProgram = asteroidShaderProgram;
        mass = mMass;
        velocity = mVelocity;
        pos = mPos;

        sizeRatio = glGetUniformLocation(shaderProgram, "sizeRatio");

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        // Specify layout of point data
        GLint posAttrib = glGetAttribLocation(shaderProgram, "pos");
        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    }

    Asteroid::~Asteroid() {}

    void Asteroid::step(GameState state, double t, double dt) {}

    void Asteroid::draw(GameState state) {}

    void Asteroid::onWorldChange(World world) {
        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glUniform2f(sizeRatio, world.getGlRatio().x, world.getGlRatio().y);
    }
}
