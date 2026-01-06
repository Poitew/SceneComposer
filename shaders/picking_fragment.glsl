#version 460 core

uniform float modelID;

out vec3 FragColor;

void main() {
    FragColor = vec3(modelID, 0.0f, 0.0f);
}