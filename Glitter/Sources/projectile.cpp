#include "projectile.hpp"

#include "game_state.hpp"
#include "util.hpp"

Asteroid::Projectile::Projectile(float pAngle, float pSpeed, glm::vec2 pPos) {
    shaderProgram = createShaderProgram(
        shaderFile("projectile.vert"), shaderFile("projectile.geo"), shaderFile("projectile.frag"));
    speed = pSpeed;
    angle = pAngle;
    pos = pPos;
    sizeRatio = glGetUniformLocation(shaderProgram, "sizeRatio");

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Specify layout of point data
    GLint posAttrib = glGetAttribLocation(shaderProgram, "pos");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

    onWorldChange(World::getInstance());
    World::getInstance().addChangeCallback(std::bind(&Asteroid::Projectile::onWorldChange, this, std::placeholders::_1));
}

Asteroid::Projectile::~Projectile() {
    glDeleteProgram(shaderProgram);
}

void Asteroid::Projectile::step(GameState state, double t, double dt) {
    float frameSpeed = speed * dt;
    pos.x += frameSpeed * cosf(angle);
    pos.y += frameSpeed * sinf(angle);
    pos = World::getInstance().wrapWorldCoord(pos);
}

void Asteroid::Projectile::draw(GameState state) {
    glUseProgram(shaderProgram);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glm::vec2 viewPos = World::getInstance().worldCoordToViewCoord(glm::vec2(pos.x, pos.y));
    float projectilePos[] = {
        viewPos.x, viewPos.y
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(projectilePos), &projectilePos, GL_STREAM_DRAW);
    glDrawArrays(GL_POINTS, 0, 1);
}

void Asteroid::Projectile::onWorldChange(World world) {
    glUseProgram(shaderProgram);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glUniform2f(sizeRatio, world.getGlRatio().x, world.getGlRatio().y);
}
