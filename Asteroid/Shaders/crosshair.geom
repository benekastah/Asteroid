#version 400 core

layout(points) in;
uniform vec2 sizeRatio;
uniform float radius;
layout(triangle_strip, max_vertices = 32) out;

void main() {
    float width = radius / 10;
    float distFromPos = radius / 3;
    float height = radius - distFromPos;
    vec4 _sizeRatio = vec4(sizeRatio, 0, 0);
    vec4 rect[16];

    rect[0] = gl_in[0].gl_Position + vec4(-width, distFromPos, 0, 0) * _sizeRatio;
    rect[1] = gl_in[0].gl_Position + vec4(width, distFromPos, 0, 0) * _sizeRatio;
    rect[2] = gl_in[0].gl_Position + vec4(-width, height + distFromPos, 0, 0) * _sizeRatio;
    rect[3] = gl_in[0].gl_Position + vec4(width, height + distFromPos, 0, 0) * _sizeRatio;

    rect[4] = gl_in[0].gl_Position + vec4(distFromPos, -width, 0, 0) * _sizeRatio;
    rect[5] = gl_in[0].gl_Position + vec4(height + distFromPos, -width, 0, 0) * _sizeRatio;
    rect[6] = gl_in[0].gl_Position + vec4(distFromPos, width, 0, 0) * _sizeRatio;
    rect[7] = gl_in[0].gl_Position + vec4(distFromPos + height, width, 0, 0) * _sizeRatio;

    rect[8] = gl_in[0].gl_Position + vec4(-width, -distFromPos, 0, 0) * _sizeRatio;
    rect[9] = gl_in[0].gl_Position + vec4(width, -distFromPos, 0, 0) * _sizeRatio;
    rect[10] = gl_in[0].gl_Position + vec4(-width, -(height + distFromPos), 0, 0) * _sizeRatio;
    rect[11] = gl_in[0].gl_Position + vec4(width, -(height + distFromPos), 0, 0) * _sizeRatio;

    rect[12] = gl_in[0].gl_Position + vec4(-distFromPos, -width, 0, 0) * _sizeRatio;
    rect[13] = gl_in[0].gl_Position + vec4(-(height + distFromPos), -width, 0, 0) * _sizeRatio;
    rect[14] = gl_in[0].gl_Position + vec4(-distFromPos, width, 0, 0) * _sizeRatio;
    rect[15] = gl_in[0].gl_Position + vec4(-(distFromPos + height), width, 0, 0) * _sizeRatio;

    for (int i = 0; i < 4; i++) {
        gl_Position = rect[(i*4)+0];
        EmitVertex();
        gl_Position = rect[(i*4)+1];
        EmitVertex();
        gl_Position = rect[(i*4)+2];
        EmitVertex();
        gl_Position = rect[(i*4)+0];
        EmitVertex();
        EndPrimitive();
        gl_Position = rect[(i*4)+1];
        EmitVertex();
        gl_Position = rect[(i*4)+2];
        EmitVertex();
        gl_Position = rect[(i*4)+3];
        EmitVertex();
        gl_Position = rect[(i*4)+1];
        EmitVertex();
        EndPrimitive();
    }
}
