#include <functional>

#include "projectile.hpp"
#include "util.hpp"

static GLuint projectileShader;
static bool shaderInitialized = false;

namespace Asteroid {

    Projectile::~Projectile() {
        delete coll;
    }

    Projectile::Projectile(float mass, float force, glm::vec2 pPos, glm::vec2 vel, glm::vec2 aim) {
        alive = true;
        if (!shaderInitialized) {
            projectileShader = createShaderProgram(
                shaderFile("projectile.vert"), shaderFile("projectile.geo"), shaderFile("projectile.frag"));
            shaderInitialized = true;
        }
        shaderProgram = projectileShader;
        float r = 0.3;
        rb = Rigidbody(mass, pPos, r);
        if (vel.x == 0 && vel.y == 0) {
            vel.x = 1;
        }
        rb.applyVelocity(vel);
        rb.applyForce(glm::normalize(aim) * glm::vec2(force, force));
        coll = new Collider(&rb, PROJECTILE);
        coll->addCollisionCallback(std::bind(&Projectile::onCollide, this, std::placeholders::_1, std::placeholders::_2));
        coll->enable();
        sizeRatio = glGetUniformLocation(shaderProgram, "sizeRatio");
        radius = glGetUniformLocation(shaderProgram, "radius");
        startTime = 0;

        glUseProgram(shaderProgram);
        glUniform1f(radius, World::getInstance().worldSizeToViewSize(r));
        glUniform4f(glGetUniformLocation(shaderProgram, "color"), 1, 1, 1, 1);

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

    void Projectile::step(double t, double dt) {
        if (!alive) {
            return;
        }
        if (startTime == 0) {
            startTime = t;
        }
        rb.step(t, dt);
    }

    void Projectile::draw() {
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

    void Projectile::onCollide(const Collider other, const InteractionType t) {
        if (t != COLLIDE) {
            return;
        }
        alive = false;
    }
}
