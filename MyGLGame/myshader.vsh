#version 410

layout (location=0) in vec3 vertex_pos;
layout (location=1) in vec3 vertex_normal;
layout (location=2) in vec4 vertex_color;
uniform vec3 light_dir;
uniform mat4 pvm_mat;
uniform mat4 model_mat;
uniform vec4 diffuse_color;
out vec4 color;

void main()
{
    gl_Position = vec4(vertex_pos, 1.0) * pvm_mat;

    vec3 normal = normalize((vec4(vertex_normal, 0.0) * model_mat).xyz);
    float power = clamp(dot(normal, -normalize(light_dir)), 0.0, 1.0);
    color = vertex_color * diffuse_color * power;
}
