#version 400 core

layout(points) in;
uniform vec2 sizeRatio;
uniform float radius;
layout(triangle_strip, max_vertices = 33) out;

const float PI = 3.1415926;

void main() {
    for (int i = 0; i <= 10; i++) {
        float a = PI * 2.0 / 10.0 * i;
        float a2 = PI * 2.0 / 10.0 * (i + 1);
        gl_Position = gl_in[0].gl_Position + vec4(cos(a) * radius, -sin(a) * radius, 0.0, 0.0) * vec4(sizeRatio, 0.0, 0.0);
        EmitVertex();

        gl_Position = gl_in[0].gl_Position + vec4(cos(a2) * radius, -sin(a2) * radius, 0.0, 0.0) * vec4(sizeRatio, 0.0, 0.0);
        EmitVertex();

        gl_Position = gl_in[0].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}
