#version 460 core
out vec4 FragColor;

in vec3 text_coord;

uniform samplerCube skybox;

void main() {    
    FragColor = texture(skybox, text_coord);
}