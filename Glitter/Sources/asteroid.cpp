#include "asteroid.hpp"

#include "btBulletDynamicsCommon.h"

#include "game_state.hpp"

namespace Asteroid {
    Asteroid::Asteroid(float mMass, glm::vec2 mPos) {
		alive = true;
		mMass = maxf(minf(mMass, ASTEROID_MASS_MAX), ASTEROID_MASS_MIN);
		shaderProgram = createShaderProgram(
			shaderFile("asteroid.vert"), shaderFile("asteroid.geo"), shaderFile("asteroid.frag"));
		rb = Rigidbody(mMass, mPos);
		float r = scale(ASTEROID_MASS_MIN, ASTEROID_MASS_MAX, 1.5, 9, rb.mass);
		coll = new Collider(&rb, r, ASTEROID);
		coll->addCollisionCallback(std::bind(&Asteroid::onCollision, this, std::placeholders::_1));
		coll->enable();

		direction = glGetUniformLocation(shaderProgram, "direction");
		sizeRatio = glGetUniformLocation(shaderProgram, "sizeRatio");
		radius = glGetUniformLocation(shaderProgram, "radius");
		rotation = glGetUniformLocation(shaderProgram, "rotation");
		color = glGetUniformLocation(shaderProgram, "color");

		glUseProgram(shaderProgram);
		glUniform1f(rotation, 1);
		glUniform1f(radius, World::getInstance().worldSizeToViewSize(r));

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

    Asteroid::~Asteroid() {
		glDeleteProgram(shaderProgram);
		delete coll;
	}

	void Asteroid::onCollision(const Collider other) {
		glUseProgram(shaderProgram);
		collided = true;
		if (other.type == PROJECTILE) {
			alive = false;
		}
	}

    void Asteroid::step(GameState state, double t, double dt) {
		if (!alive) {
			return;
		}
		glUseProgram(shaderProgram);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		rb.step(state, t, dt);
		float dir = atan2f(rb.velocity.y, rb.velocity.x);
		glUniform1f(direction, dir);
		if (collided) {
			glUniform4f(color, 1, 0, 0, 1);
			collided = false;
		} else {
			glUniform4f(color, 1, 1, 1, 1);
		}
	}

    void Asteroid::draw(GameState state) {
		if (!alive) {
			return;
		}
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
