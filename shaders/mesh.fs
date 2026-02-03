#version 460 core
in vec3 FragPos;
in vec3 Normal;
in vec2 text_coord;

out vec4 FragColor;

uniform sampler2D texture_diffuse;
uniform vec4 material_color;
uniform vec3 light_pos;
uniform vec3 view_pos;
uniform bool use_texture;

void main() {
    vec3 obj_color;
    if (use_texture) {
        obj_color = texture(texture_diffuse, text_coord).rgb * material_color.rgb;
    } else {
        obj_color = material_color.rgb;
    }

    float ambient_strength = 0.35;
    vec3 light_color = vec3(1.0, 1.0, 1.0);
    vec3 ambient = ambient_strength * light_color;

    vec3 norm = normalize(Normal);
    vec3 light_direction = normalize(light_pos - FragPos);

    float diff = max(dot(norm, light_direction), 0.0);
    vec3 diffuse = diff * light_color;

    float specular_strength = 0.5;
    vec3 view_dir = normalize(view_pos - FragPos);
    vec3 reflection_dir = reflect(-light_direction, norm);
    float spec = pow(max(dot(view_dir, reflection_dir), 0.0), 32);
    vec3 specular = specular_strength * spec * light_color;

    vec3 result = (ambient + diffuse + specular) * obj_color;

    FragColor = vec4(result, 1.0f);
}