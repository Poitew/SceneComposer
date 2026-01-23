#version 460 core
in vec2 text_coord;
out vec4 FragColor;

uniform sampler2D texture_diffuse;
uniform vec4 material_color;
uniform bool use_texture;

void main() {
    if (use_texture) {
        FragColor = texture(texture_diffuse, text_coord) * material_color;
    } else {
        FragColor = material_color;
    }
}