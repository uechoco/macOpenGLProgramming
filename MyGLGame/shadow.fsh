#version 410

uniform vec4 shadow_color;
layout (location=0) out vec4 frag_color;

void main()
{
    frag_color = shadow_color;
}

