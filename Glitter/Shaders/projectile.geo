#version 400 core

uniform vec2 sizeRatio;
layout(points) in;
layout(triangle_strip, max_vertices = 33) out;

const float PI = 3.1415926;

void main() {
    float r = 0.004;
    for (int i = 0; i <= 10; i++) {
        float a = PI * 2.0 / 10.0 * i;
        float a2 = PI * 2.0 / 10.0 * (i + 1);
        gl_Position = gl_in[0].gl_Position + vec4(cos(a) * r, -sin(a) * r, 0.0, 0.0) * vec4(sizeRatio, 0.0, 0.0);
        EmitVertex();

        gl_Position = gl_in[0].gl_Position + vec4(cos(a2) * r, -sin(a2) * r, 0.0, 0.0) * vec4(sizeRatio, 0.0, 0.0);
        EmitVertex();

        gl_Position = gl_in[0].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}
