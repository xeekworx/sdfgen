#version 400

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texcoord;

out vec4 pass_color;
out vec2 pass_texcoord;

void main() {
    gl_Position = position;
    pass_texcoord = texcoord;
    pass_color = color;
}