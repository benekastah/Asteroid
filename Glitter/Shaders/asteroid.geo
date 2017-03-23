#version 400 core

layout(points) in;
uniform float direction;
uniform float rotation;
uniform float radius;
uniform vec2 sizeRatio;
layout(line_strip, max_vertices = 14) out;

const float PI = 3.1415926;

void main() {
    float totalRadians = 2 * PI;
    float advanceBy = totalRadians / 7;
    float a = direction;

    vec4 pa = gl_in[0].gl_Position + vec4(radius * cos(a), radius * sin(a), 0.0, 0.0) * vec4(sizeRatio, 0.0, 0.0);
	gl_Position = pa;
    EmitVertex();
    a += advanceBy;
    gl_Position = gl_in[0].gl_Position + vec4(radius * cos(a), radius * sin(a), 0.0, 0.0) * vec4(sizeRatio, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();

    EmitVertex();
    a += advanceBy;
    gl_Position = gl_in[0].gl_Position + vec4(radius * cos(a), radius * sin(a), 0.0, 0.0) * vec4(sizeRatio, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();

    EmitVertex();
    a += advanceBy;
    gl_Position = gl_in[0].gl_Position + vec4(radius * cos(a), radius * sin(a), 0.0, 0.0) * vec4(sizeRatio, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();

    EmitVertex();
    a += advanceBy;
    gl_Position = gl_in[0].gl_Position + vec4(radius * cos(a), radius * sin(a), 0.0, 0.0) * vec4(sizeRatio, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();

    EmitVertex();
    a += advanceBy;
    gl_Position = gl_in[0].gl_Position + vec4(radius * cos(a), radius * sin(a), 0.0, 0.0) * vec4(sizeRatio, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();

    EmitVertex();
    a += advanceBy;
    gl_Position = gl_in[0].gl_Position + vec4(radius * cos(a), radius * sin(a), 0.0, 0.0) * vec4(sizeRatio, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();

    EmitVertex();
	gl_Position = pa;
    EmitVertex();
    EndPrimitive();
}
