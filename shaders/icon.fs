#version 460 core
out vec4 FragColor;

in vec2 text_coord;
uniform sampler2D texture_diffuse;

void main() {
    vec4 sampled = texture(texture_diffuse, text_coord);

    if(sampled.a < 0.1) discard;
    
    FragColor = sampled;
}