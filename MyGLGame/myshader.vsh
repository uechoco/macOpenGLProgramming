#version 410

layout (location=0) in vec3 vertex_pos;
layout (location=1) in vec3 vertex_normal;
layout (location=2) in vec4 vertex_color;
uniform vec3 light_pos;
uniform float light_attenuation;
uniform vec3 spot_dir;
uniform float spot_phi;
uniform float spot_theta;
uniform float spot_falloff;
uniform vec3 eye_dir;
uniform mat4 pvm_mat;
uniform mat4 model_mat;
uniform vec4 diffuse_color;
uniform vec4 ambient_color;
uniform vec4 specular_color;
uniform float specular_shininess;
uniform vec4 emissive_color;
out vec3 v_position;
out vec3 v_normal;
out vec4 v_color;

void main()
{
    v_position = (vec4(vertex_pos, 1.0) * model_mat).xyz;
    v_normal = vertex_normal;
    v_color = vertex_color;
    gl_Position = vec4(vertex_pos, 1.0) * pvm_mat;
}
