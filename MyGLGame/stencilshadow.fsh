#version 410

in vec3 v_color;
layout (location=0) out vec4 frag_color;

void main()
{
    frag_color = vec4(v_color, 1.0);
    //frag_color = vec4(1.0, 1.0, 1.0, 1.0);
}

