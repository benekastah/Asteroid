#version 400 core

layout(points) in;
uniform float direction;
uniform vec2 sizeRatio;
layout(triangle_strip, max_vertices = 3) out;

void main()
{
    float size = 0.03;
    mat2 rotate = mat2(cos(direction), sin(direction),
                       -sin(direction), cos(direction));

    gl_Position = gl_in[0].gl_Position + vec4(vec2(-size, -size) * rotate * sizeRatio, 0.0, 0.0);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(vec2(size, -size) * rotate * sizeRatio, 0.0, 0.0);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(vec2(0.0, size) * rotate * sizeRatio, 0.0, 0.0);
    EmitVertex();

    EndPrimitive();
}
