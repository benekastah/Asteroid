#version 400 core

layout(points) in;
uniform vec2 sizeRatio;
uniform float radius;
layout(triangle_strip, max_vertices = 33) out;

const float PI = 3.1415926;
const float TAU = PI * 2.0;

void main() {
    int numSegments = 10;
    for (int i = 0; i < numSegments; i++) {
        float a1 = TAU / numSegments * i;
        float a2 = TAU / numSegments * (i + 1);
        gl_Position = gl_in[0].gl_Position + vec4(radius * cos(a1), radius * sin(a1), 0.0, 0.0) * vec4(sizeRatio, 0.0, 0.0);
        EmitVertex();

        gl_Position = gl_in[0].gl_Position + vec4(radius * cos(a2), radius * sin(a2), 0.0, 0.0) * vec4(sizeRatio, 0.0, 0.0);
        EmitVertex();

        gl_Position = gl_in[0].gl_Position;
        EmitVertex();
        EndPrimitive();
    }
}
