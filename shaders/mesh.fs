#version 460 core
in vec2 text_coord;
out vec4 FragColor;

uniform sampler2D texture_diffuse;

void main() {
    FragColor = texture(texture_diffuse, text_coord);
}