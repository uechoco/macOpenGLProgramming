#version 410

in vec4 color;
in vec2 uv;
uniform sampler2D tex;
layout (location=0) out vec4 frag_color;

void main()
{
    // ベクトル同士の掛け算は、各成分同士の掛け算(v.x = v1.x * v2.x)
    frag_color = texture(tex, uv) * color;
}
