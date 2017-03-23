#include "player.hpp"

#include <math.h>

#include "world.hpp"
#include "game_state.hpp"

namespace Asteroid {
	Player::Player() {
		shaderProgram = createShaderProgram(
			shaderFile("player.vert"), shaderFile("player.geo"), shaderFile("player.frag"));
		rb = Rigidbody(5000, glm::vec2(50, 50));
		rb.maxVelocity = 100;
		float r = 1.5;
		coll = new Collider(&rb, r, PLAYER);
		direction = glGetUniformLocation(shaderProgram, "direction");
		radius = glGetUniformLocation(shaderProgram, "radius");
		sizeRatio = glGetUniformLocation(shaderProgram, "sizeRatio");

		glUseProgram(shaderProgram);
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
		World::getInstance().addChangeCallback(std::bind(&Player::onWorldChange, this, std::placeholders::_1));

		gun.initialize(5, 1.2, 3, 0.5);
	}

	Player::~Player() {
		glDeleteProgram(shaderProgram);
	}

	void Player::step(GameState state, double t, double dt) {
		glUseProgram(shaderProgram);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		const float maxForce = 50000 * 50;
		const float forcePerStep = maxForce / 10;
		auto window = state.window;

		auto force = glm::vec2(0, 0);
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			force.y += forcePerStep;
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			force.y -= forcePerStep;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			force.x -= forcePerStep;
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			force.x += forcePerStep;
		}
		force.x = minf(maxForce, maxf(-maxForce, force.x));
		force.y = minf(maxForce, maxf(-maxForce, force.y));

		rb.applyForce(force);
		rb.step(state, t, dt);
		float dir = atan2f(rb.velocity.y, rb.velocity.x);
		glUniform1f(direction, dir);

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			gun.fireBullet(t, rb.velocity, rb.pos);
		}

		gun.step(state, t, dt);
	}

	void Player::draw(GameState state) {
		glUseProgram(shaderProgram);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		World world = World::getInstance();
		auto playerPos = world.getPositionBuffer(rb.pos);
		glBufferData(GL_ARRAY_BUFFER, sizeof(playerPos), &playerPos.buf, GL_STREAM_DRAW);
		glDrawArrays(GL_POINTS, 0, 9);

		gun.draw(state);
	}

	void Player::onWorldChange(World world) {
		glUseProgram(shaderProgram);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glUniform2f(sizeRatio, world.getGlRatio().x, world.getGlRatio().y);
	}
}
