#include <functional>

#include "projectile.hpp"

#include "game_state.hpp"
#include "util.hpp"

static GLuint projectileShader;
static bool shaderInitialized = false;

namespace Asteroid {
	void Projectile::initialize(glm::vec2 pPos, glm::vec2 vel) {
		alive = true;
		if (!shaderInitialized) {
			projectileShader = createShaderProgram(
				shaderFile("projectile.vert"), shaderFile("projectile.geo"), shaderFile("projectile.frag"));
			shaderInitialized = true;
		}
		shaderProgram = projectileShader;
		float r = 0.3;
		rb = Rigidbody(25, pPos);
		if (vel.x == 0 && vel.y == 0) {
			vel.x = 1;
		}
		rb.applyVelocity(vel);
		rb.applyForce(glm::normalize(vel) * glm::vec2(BULLET_FORCE, BULLET_FORCE));
		coll = new Collider(&rb, r, PROJECTILE);
		coll->addCollisionCallback(std::bind(&Projectile::onCollide, this, std::placeholders::_1));
		sizeRatio = glGetUniformLocation(shaderProgram, "sizeRatio");
		radius = glGetUniformLocation(shaderProgram, "radius");
		startTime = 0;

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
		World::getInstance().addChangeCallback(std::bind(&Projectile::onWorldChange, this, std::placeholders::_1));
	}

	void Projectile::step(GameState state, double t, double dt) {
		if (!alive) {
			return;
		}
		if (startTime == 0) {
			startTime = t;
		}
		rb.step(state, t, dt);
	}

	void Projectile::draw(GameState state) {
		if (!alive) {
			return;
		}
		glUseProgram(shaderProgram);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glm::vec2 viewPos = World::getInstance().worldCoordToViewCoord(rb.pos);
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

	void Projectile::onCollide(const Collider coll) {
		alive = false;
	}


}
