#version 410

layout (location=0) in vec3 vertex_pos;
layout (location=1) in vec3 vertex_normal;
layout (location=2) in vec3 vertex_color;
uniform vec3 light_pos;
uniform vec3 spot_dir;
uniform float far;
uniform mat4 pvm_mat;
uniform mat4 model_mat;
out vec3 v_color;

void main()
{
    vec3 v_position = (vec4(vertex_pos, 1.0) * model_mat).xyz;
    v_color = vertex_color;
    vec3 moved_pos = vertex_pos;
    
    vec3 light_dir = spot_dir;
    vec3 light_vec_dir = v_position - light_pos;
    // 頂点法線が光源ベクトルと鋭角（＝だいたい同じ方向）の頂点を
    // 遠方に移動させる
    float cos_alpha = dot(light_dir, vertex_normal);
    if (cos_alpha >= 0)
    {
        vec3 light_dirN = normalize(light_dir);
        float light_len = length(light_vec_dir);
        // 遠方に移動させるベクトル
        vec3 move_vec = (far - light_len) * light_dirN;
        moved_pos += move_vec;
    }
    
    gl_Position = vec4(moved_pos, 1.0) * pvm_mat;
}

