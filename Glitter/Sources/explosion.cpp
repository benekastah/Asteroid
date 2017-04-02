
#include "explosion.hpp"
#include "util.hpp"

namespace Asteroid {

    Explosion::Explosion(glm::vec2 mPos, glm::vec4 mColor, float mMass, float mDensity, double mTimeToLive) {
        alive = true;
        pos = mPos;
        mass = mMass;
        density = mDensity;
        color = mColor;
        timeToLive = mTimeToLive;
        startTime = 0;

        shaderProgram = createShaderProgram(
            shaderFile("explosion.vert"), shaderFile("explosion.geo"), shaderFile("explosion.frag"));

        colorUniform = glGetUniformLocation(shaderProgram, "color");
        radiusUniform = glGetUniformLocation(shaderProgram, "radius");
        sizeRatio = glGetUniformLocation(shaderProgram, "sizeRatio");

        radius = findRadius(mass, density);
        for (int i = 0; i < randfBtwn(3, 5); i++) {
            auto rb = new Rigidbody(mass, mPos, radius);
            auto dest = pointOnCircle(mPos, 100, randfBtwn(0, 2 * PI));
            rb->applyVelocity(glm::normalize(dest - rb->pos) * toVec2(randfBtwn(5, 13)));
            balls.push_back(rb);
        }

        auto world = World::getInstance();
        glUseProgram(shaderProgram);
        glUniform4f(colorUniform, mColor.r, mColor.g, mColor.b, 0.2);
        glUniform1f(radiusUniform, world.worldSizeToViewSize(radius));

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glGenTextures(1, &tex);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        auto texColor = glm::vec4(1, 1, 1, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_FLOAT, &texColor);
        glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 0);

        // Specify layout of point data
        size_t vertexRowSize = 2 * sizeof(float);

        GLint posAttrib = glGetAttribLocation(shaderProgram, "pos");
        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, vertexRowSize, 0);

        onWorldChange(world);
        world.addChangeCallback(std::bind(&Explosion::onWorldChange, this, std::placeholders::_1));
    }

    Explosion::~Explosion() {
        glDeleteProgram(shaderProgram);
        for each (auto rb in balls) {
            delete rb;
        }
    }

    void Explosion::step(double t, double dt) {
        if (startTime == 0) {
            startTime = t;
        }
        if (timeToLive - (t - startTime) < 0) {
            alive = false;
            return;
        }
        glUseProgram(shaderProgram);
        density *= 0.95;
        radius = findRadius(mass, density);
        glUniform1f(radiusUniform, World::getInstance().worldSizeToViewSize(radius));
        if (alive) {
            for each (auto rb in balls) {
                rb->step(t, dt);
                rb->radius = radius;
            }
        }
    }

    void Explosion::draw() {
        if (!alive) {
            return;
        }
        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        std::vector<glm::vec2> buf;
        for each (auto rb in balls) {
            glm::vec2 viewPos = World::getInstance().worldCoordToViewCoord(rb->pos);
            buf.push_back(viewPos);
        }
        glBufferData(GL_ARRAY_BUFFER, buf.size() * sizeof(buf[0]), &buf[0], GL_STREAM_DRAW);
        glDrawArrays(GL_POINTS, 0, balls.size());
    }

    void Explosion::onWorldChange(World world) {
        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glUniform2f(sizeRatio, world.getGlRatio().x, world.getGlRatio().y);
    }
}
