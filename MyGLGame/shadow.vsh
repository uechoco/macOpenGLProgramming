#version 410

layout (location=0) in vec3 vertex_pos;
layout (location=1) in vec3 vertex_normal;
uniform vec4 shadow_color;

void main()
{
    gl_Position = vec4(vertex_pos, 1.0);
}

