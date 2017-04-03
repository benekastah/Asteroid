#version 400 core

layout(points) in;
uniform float direction;
uniform float rotation;
uniform float radius;
uniform vec2 sizeRatio;
uniform float warp[14];

layout(line_strip, max_vertices = 14) out;

const float PI = 3.1415926;

void main() {
    float totalRadians = 2 * PI;
    float advanceBy = totalRadians / 7;
    float a = direction;
    float warpSize = radius / 6;
    vec4 _sizeRatio = vec4(sizeRatio, 0.0, 0.0);

    vec4 first;
    for (int i = 0; i < 7; i++) {
        int wi = i * 2;
        vec4 vertex = gl_in[0].gl_Position + vec4(radius * cos(a) + warp[wi] * warpSize, radius * sin(a) + warp[wi + 1] * warpSize, 0.0, 0.0) * _sizeRatio;
        gl_Position = vertex;
        if (i == 0) {
            first = vertex;
        }
        EmitVertex();
        if (i % 2 != 0) {
            EndPrimitive();
            EmitVertex();
        }
        a += advanceBy;
    }
    gl_Position = first;
    EmitVertex();
    EndPrimitive();
}
