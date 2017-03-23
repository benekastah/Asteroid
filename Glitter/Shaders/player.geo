#version 400 core

layout(points) in;
uniform float direction;
uniform float radius;
uniform vec2 sizeRatio;
layout(triangle_strip, max_vertices = 6) out;

const float PI = 3.1415926;

void main() {
    //float r = 0.03;
    float totalRadians = 2 * PI;
    float advanceBy = totalRadians / 3;
    float a = direction;

    vec4 top = gl_in[0].gl_Position + vec4(radius * cos(a), radius * sin(a), 0.0, 0.0) * vec4(sizeRatio, 0.0, 0.0);
    a += advanceBy;
    vec4 bottom1 = gl_in[0].gl_Position + vec4(radius * cos(a), radius * sin(a), 0.0, 0.0) * vec4(sizeRatio, 0.0, 0.0);
    a += advanceBy;
    vec4 bottom2 = gl_in[0].gl_Position + vec4(radius * cos(a), radius * sin(a), 0.0, 0.0) * vec4(sizeRatio, 0.0, 0.0);

    gl_Position = top;
    EmitVertex();
    gl_Position = bottom1;
    EmitVertex();
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    gl_Position = top;
    EmitVertex();
    gl_Position = bottom2;
    EmitVertex();
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    EndPrimitive();
}
