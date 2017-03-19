#include "asteroid.hpp"

#include "btBulletDynamicsCommon.h"

#include "game_state.hpp"

namespace Asteroid {
    Asteroid::Asteroid(float mMass, glm::vec2 mPos) {
		mMass = maxf(minf(mMass, ASTEROID_MASS_MAX), ASTEROID_MASS_MIN);
		shaderProgram = createShaderProgram(
			shaderFile("asteroid.vert"), shaderFile("asteroid.geo"), shaderFile("asteroid.frag"));
		rb = Rigidbody(mMass, mPos);

		direction = glGetUniformLocation(shaderProgram, "direction");
		sizeRatio = glGetUniformLocation(shaderProgram, "sizeRatio");
		size = glGetUniformLocation(shaderProgram, "size");
		rotation = glGetUniformLocation(shaderProgram, "rotation");

		glUseProgram(shaderProgram);
		glUniform1f(rotation, 1);
		float s = scale(ASTEROID_MASS_MIN, ASTEROID_MASS_MAX, 3, 18, rb.mass);
		glUniform1f(size, World::getInstance().worldSizeToViewSize(s));

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        // Specify layout of point data
        GLint posAttrib = glGetAttribLocation(shaderProgram, "pos");
        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

		onWorldChange(World::getInstance());
		World::getInstance().addChangeCallback(std::bind(&Asteroid::onWorldChange, this, std::placeholders::_1));
    }

    Asteroid::~Asteroid() {}

    void Asteroid::step(GameState state, double t, double dt) {
		glUseProgram(shaderProgram);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		rb.step(state, t, dt);
		float dir = atan2f(rb.velocity.y, rb.velocity.x);
		glUniform1f(direction, dir);
	}

    void Asteroid::draw(GameState state) {
		glUseProgram(shaderProgram);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		World world = World::getInstance();
		auto asteroidPos = world.getPositionBuffer(rb.pos);
		glBufferData(GL_ARRAY_BUFFER, sizeof(asteroidPos), &asteroidPos.buf, GL_STREAM_DRAW);
		glDrawArrays(GL_POINTS, 0, 9);
	}

    void Asteroid::onWorldChange(World world) {
        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glUniform2f(sizeRatio, world.getGlRatio().x, world.getGlRatio().y);
    }
}
