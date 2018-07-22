#version 410

in vec4 color;
in vec2 uv;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform float t;
layout (location=0) out vec4 frag_color;

void main()
{
    vec4 c1 = texture(tex1, uv);
    vec4 c2 = texture(tex2, uv);
    frag_color = (uv.x < t? c1: c2);
}
