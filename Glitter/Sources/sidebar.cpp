#include <functional>

#include "sidebar.hpp"
#include "game_state.hpp"
#include "world.hpp"

namespace Asteroid {
	Sidebar::Sidebar() {
		shaderProgram = createShaderProgram(shaderFile("sidebar.vert"), shaderFile("sidebar.frag"));
		// Create Vertex Array Object
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		// Create a Vertex Buffer Object and copy the vertex data to it
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		// Create an element array
		glGenBuffers(1, &ebo);

		GLuint elements[] = {
			0, 1, 2,
			2, 3, 0,

			4, 5, 6,
			6, 7, 4,

			8, 9, 10,
			10, 11, 8,

			12, 13, 14,
			14, 15, 12,
		};
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

		// Specify the layout of the vertex data
		GLint posAttrib = glGetAttribLocation(shaderProgram, "pos");
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

		auto world = World::getInstance();
		onWorldChange(world);
		world.addChangeCallback(std::bind(&Sidebar::onWorldChange, this, std::placeholders::_1));
	}

	Sidebar::~Sidebar() {
		glDeleteProgram(shaderProgram);
	}

	void Sidebar::step(GameState state, double dt, double t) {}

	void Sidebar::draw(GameState state) {
		auto world = World::getInstance();
		glUseProgram(shaderProgram);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		for (int i = 0; i < 4; i++) {
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *) (i * 6 * sizeof(GLuint)));
		}
	}

	void Sidebar::onWorldChange(World world) {
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		auto glBounds = world.glBounds();
		GLfloat vertices[] = {
			// Left triangle
			-1,  1, // Top-left
			glBounds[0].x, 1, // Top-right
			glBounds[0].x, -1, // Bottom-right
			-1, -1,  // Bottom-left

			// Top triangle
			-1,  1, // Top-left
			1, 1, // Top-right
			1, glBounds[1].y, // Bottom-right
			-1, glBounds[1].y,  // Bottom-left

			// Right triangle
			glBounds[1].x, 1, // Top-left
			1, 1, // Top-right
			1, -1, // Bottom-right
			glBounds[1].x, -1,  // Bottom-left

			// Bottom triangle
			-1,  glBounds[0].y, // Top-left
			1, glBounds[0].y, // Top-right
			1, -1, // Bottom-right
			-1, -1,  // Bottom-left
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	}
}
