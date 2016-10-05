#version 400 core

layout(points) in;
uniform float direction;
uniform vec2 sizeRatio;
layout(triangle_strip, max_vertices = 3) out;

const float PI = 3.1415926;

void main() {
    float r = 0.03;
    float totalRadians = 2 * PI;
    float advanceBy = totalRadians / 3;

    float a = direction;
    for (int i = 0; i < 3; i++) {
        gl_Position = gl_in[0].gl_Position + vec4(r * cos(a), r * sin(a), 0.0, 0.0) * vec4(sizeRatio, 0.0, 0.0);
        EmitVertex();
        a += advanceBy;
    }
    EndPrimitive();
}
