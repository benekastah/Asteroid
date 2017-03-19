#include "projectile.hpp"

#include "game_state.hpp"
#include "util.hpp"

static GLuint projectileShader;
static bool shaderInitialized = false;

namespace Asteroid {
	void Projectile::initialize(float pAngle, float pSpeed, glm::vec2 pPos) {
		if (!shaderInitialized) {
			projectileShader = createShaderProgram(
				shaderFile("projectile.vert"), shaderFile("projectile.geo"), shaderFile("projectile.frag"));
			shaderInitialized = true;
		}
		shaderProgram = projectileShader;
		speed = pSpeed;
		angle = pAngle;
		pos = pPos;
		sizeRatio = glGetUniformLocation(shaderProgram, "sizeRatio");
		startTime = 0;

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		// Specify layout of point data
		GLint posAttrib = glGetAttribLocation(shaderProgram, "pos");
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

		onWorldChange(World::getInstance());
		World::getInstance().addChangeCallback(std::bind(&Projectile::onWorldChange, this, std::placeholders::_1));
	}

	void Projectile::step(GameState state, double t, double dt) {
		if (startTime == 0) {
			startTime = t;
		}
		float frameSpeed = speed * dt;
		pos.x += frameSpeed * cosf(angle);
		pos.y += frameSpeed * sinf(angle);
		pos = World::getInstance().wrapWorldCoord(pos);
	}

	void Projectile::draw(GameState state) {
		glUseProgram(shaderProgram);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glm::vec2 viewPos = World::getInstance().worldCoordToViewCoord(glm::vec2(pos.x, pos.y));
		float projectilePos[] = {viewPos.x, viewPos.y};
		glBufferData(GL_ARRAY_BUFFER, sizeof(projectilePos), &projectilePos, GL_STREAM_DRAW);
		glDrawArrays(GL_POINTS, 0, 1);
	}

	void Projectile::onWorldChange(World world) {
		glUseProgram(shaderProgram);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glUniform2f(sizeRatio, world.getGlRatio().x, world.getGlRatio().y);
	}
}
