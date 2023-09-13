#version 450

layout(location = 0) out vec4 f_FragColor;

layout(location = 0) in vec2 f_TexCoord;
layout(location = 1) in vec4 f_Color;

void main() {
    f_FragColor = f_Color;
}