#include "glitter.hpp"

#include <GLFW/glfw3.h>
#include <math.h>

#include <glm/vec2.hpp>

#include "util.hpp"

#define MAX_NUM_SHADERS 100

char* readFile(const char* fname) {
    char* buffer = NULL;
    long length;

    FILE* f;
    errno_t err = fopen_s(&f, fname, "rb");
    if (err != 0) {
        fprintf(stderr, "Error reading file \"%s\"\n", fname);
        exit(1);
    }

    if (f) {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = (char *)calloc(1, length + 1);
        if (buffer) {
            fread(buffer, 1, length, f);
        }
        fclose(f);
    }

    return buffer;
}

float minf(float a, float b) {
    return a < b ? a : b;
}

float maxf(float a, float b) {
    return a > b ? a : b;
}

float scale(float min1, float max1, float min2, float max2, float val) {
    float percent = (val - min1) / (max1 - min1);
    return percent * (max2 - min2) + min2;
}

void _srand() {
    static bool initialized = false;
    if (!initialized) {
        srand((unsigned int)(glfwGetTime() * 1e9));
    }
}

float randf() {
    _srand();
    return (float) rand();
}

float randfBtwn(float min, float max) {
    return scale(0, RAND_MAX, min, max, randf());
}

glm::vec2 toVec2(float x) {
    return glm::vec2(x, x);
}

glm::vec2 randVec2() {
    return randVec2(1);
}

glm::vec2 randVec2(float magnitude) {
    return glm::normalize(glm::vec2(randf(), randf())) * toVec2(magnitude);
}

glm::vec2 randVec2(float min, float max) {
    return glm::vec2(randfBtwn(min, max), randfBtwn(min, max));
}

glm::vec2 pointOnCircle(glm::vec2 pos, float r, float dir) {
    return pos + glm::vec2(r * cosf(dir), r * sinf(dir));
}

glm::vec2 vec2Mod(glm::vec2 a, glm::vec2 b) {
    a.x = fmodf(a.x, b.x);
    a.y = fmodf(a.y, b.y);
    return a;
}

glm::vec2 vec2Abs(glm::vec2 vec) {
    vec.x = fabsf(vec.x);
    vec.y = fabsf(vec.y);
    return vec;
}

glm::vec2 wrap(glm::vec2 limit, glm::vec2 vec) {
    return vec2Abs(vec2Mod(limit + vec, limit));
}

float wrappedDistance(float limit, float a, float b) {
    // Describes distance between two positions in a geometry where it is only possible to travel to any
    // position between [0, limit). When you reach either boundary, your position wraps around. So, if you
    // are at position zero and you move backward one position, you will end up at position limit - 1.
    return limit / 2 - fabsf(limit / 2 - fmodf(fabsf(a - b), limit));
}

float wrappedSubtract(float limit, float a, float b) {
    return wrappedDistance(limit, a, b) * sgn(a - b);
}

glm::vec2 wrappedSubtract(glm::vec2 limit, glm::vec2 a, glm::vec2 b) {
    auto sub = glm::vec2(
        wrappedSubtract(limit.x, a.x, b.x),
        wrappedSubtract(limit.y, a.y, b.y));
    if (a - b != sub) {
        return sub * toVec2(-1);
    } else {
        return sub;
    }
}

float wrappedDistance(glm::vec2 limit, glm::vec2 a, glm::vec2 b) {
    return glm::length(glm::vec2(
        wrappedDistance(limit.x, a.x, b.x),
        wrappedDistance(limit.y, a.y, b.y)));
}

bool isWrapped(float limit, float val) {
    return val >= 0 && val < limit;
}

bool isWrapped(glm::vec2 limit, glm::vec2 val) {
    return isWrapped(limit.x, val.x) && isWrapped(limit.y, val.y);
}

float distance(glm::vec2 limit, glm::vec2 a, glm::vec2 b) {
    if (isWrapped(limit, a) && isWrapped(limit, b)) {
        return wrappedDistance(limit, a, b);
    } else {
        return glm::distance(a, b);
    }
}

glm::vec2 subtract(glm::vec2 limit, glm::vec2 a, glm::vec2 b) {
    if (isWrapped(limit, a) && isWrapped(limit, b)) {
        return wrappedSubtract(limit, a, b);
    } else {
        return a - b;
    }
}

float findRadius(float mass, float density) {
    // Derived from:
    //   mass = volume * density
    //   volume of sphere = (4 * PI * r^3) / 3
    auto num = 3 * (1 / density * mass);
    auto denum = 4 * PI;
    return std::cbrtf(num / denum);
}

bool strEndsWith(const char * str, const char * ending) {
    int strL = strlen(str);
    int endingL = strlen(ending);
    if (endingL > strL) {
        return false;
    }
    str = str + (strL - endingL);
    return strcmp(str, ending) == 0;
}

void print_gl_errors() {
    GLenum glErr;
    while ((glErr = glGetError()) != GL_NO_ERROR) {
        if (glErr == GL_INVALID_ENUM) {
            printf("GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag\n");
        }
        if (glErr == GL_INVALID_VALUE) {
            printf("GL_INVALID_VALUE: A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag\n");
        }
        if (glErr == GL_INVALID_OPERATION) {
            printf("GL_INVALID_OPERATION: The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag\n");
        }
        if (glErr == GL_INVALID_FRAMEBUFFER_OPERATION) {
            printf("GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag\n");
        }
        if (glErr == GL_OUT_OF_MEMORY) {
            printf("GL_OUT_OF_MEMORY: There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded\n");
        }
        if (glErr == GL_STACK_UNDERFLOW) {
            printf("GL_STACK_UNDERFLOW: An attempt has been made to perform an operation that would cause an internal stack to underflow\n");
        }
        if (glErr == GL_STACK_OVERFLOW) {
            printf("GL_STACK_OVERFLOW: An attempt has been made to perform an operation that would cause an internal stack to overflow\n");
        }
    }
}

GLuint createShader(GLenum type, const char * fname) {
    char * src = readFile(fname);
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, (const GLchar **)&src, nullptr);
    glCompileShader(shader);

    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        //The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

        //We don't need the shader anymore.
        glDeleteShader(shader);

        for (int i = 0; i < maxLength; i++) {
            putchar(infoLog[i]);
        }
        putchar('\n');
        getchar();

        exit(1);
    }

    free(src);
    return shader;
}

GLuint _createShaderProgram(const char * name, ...) {
    va_list args;
    va_start(args, name);

    GLuint shaderProgram = glCreateProgram();

    GLuint shader;
    while (name != nullptr) {
        shader = 0;
        if (strEndsWith(name, ".frag")) {
            shader = createShader(GL_FRAGMENT_SHADER, name);
        } else if (strEndsWith(name, ".vert")) {
            shader = createShader(GL_VERTEX_SHADER, name);
        } else if (strEndsWith(name, ".geom")) {
            shader = createShader(GL_GEOMETRY_SHADER, name);
        }
        if (shader != 0) {
            glAttachShader(shaderProgram, shader);
            // glDeleteShader(shader);
        }
        name = va_arg(args, char *);
    }

    va_end(args);

    glLinkProgram(shaderProgram);

    GLint isLinked = 0;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, (int *) &isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);

        //The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(shaderProgram, maxLength, &maxLength, &infoLog[0]);

        deleteShaderProgram(shaderProgram);

        for (int i = 0; i < maxLength; i++) {
            putchar(infoLog[i]);
        }
        putchar('\n');
        getchar();

        exit(1);
    }
    return shaderProgram;
}

void deleteShaderProgram(GLuint shaderProgram) {
    GLsizei count;
    GLuint shaders[MAX_NUM_SHADERS];
    glGetAttachedShaders(shaderProgram, MAX_NUM_SHADERS, &count, &shaders[0]);
    for (int i = 0; i < count; i++) {
        glDeleteShader(shaders[i]);
    }
    glDeleteProgram(shaderProgram);
}
