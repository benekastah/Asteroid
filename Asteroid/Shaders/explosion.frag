#version 400 core

out vec4 outColor;

uniform vec4 color;
uniform sampler2D tex;

void main() {
    vec2 texcoord = vec2(0.0, 0.0);
    outColor = texture(tex, texcoord) * color;
}
