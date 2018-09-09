#version 410

layout (location=0) in vec3 vertex_pos;
layout (location=1) in vec3 vertex_normal;
layout (location=2) in vec4 vertex_color;
uniform vec3 light_dir;
uniform vec3 eye_dir;
uniform mat4 pvm_mat;
uniform mat4 model_mat;
uniform vec4 diffuse_color;
uniform vec4 ambient_color;
uniform vec4 specular_color;
uniform float specular_shininess;
out vec4 color;

void main()
{
    gl_Position = vec4(vertex_pos, 1.0) * pvm_mat;

    vec3 normal = normalize((vec4(vertex_normal, 0.0) * model_mat).xyz);
    vec3 light = -normalize(light_dir);
    float diffuse_power = clamp(dot(normal, light), 0.0, 1.0);
    vec3 eye = -normalize(eye_dir);
    vec3 half_vec = normalize(light + eye);
    float specular = pow(clamp(dot(normal, half_vec), 0.0, 1.0), specular_shininess);
    color = vertex_color * diffuse_color * diffuse_power + ambient_color + specular_color * specular;
}
