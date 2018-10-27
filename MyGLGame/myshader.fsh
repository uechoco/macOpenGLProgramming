#version 410

in vec3 v_position;
in vec3 v_normal;
in vec4 v_color;
const int LIGHT_NUM = 3;
uniform vec3 light_pos[LIGHT_NUM];
uniform float light_attenuation[LIGHT_NUM];
uniform vec3 spot_dir[LIGHT_NUM];
uniform float spot_phi[LIGHT_NUM];
uniform float spot_theta[LIGHT_NUM];
uniform float spot_falloff[LIGHT_NUM];
uniform vec3 eye_dir;
uniform mat4 pvm_mat;
uniform mat4 model_mat;
uniform vec4 diffuse_color[LIGHT_NUM];
uniform vec4 ambient_color[LIGHT_NUM];
uniform vec4 specular_color[LIGHT_NUM];
uniform float specular_shininess[LIGHT_NUM];
uniform vec4 emissive_color;
layout (location=0) out vec4 frag_color;

void main()
{
    vec4 diffuse_total = vec4(0);
    vec4 ambient_total = vec4(0);
    vec4 specular_total = vec4(0);
    for (int i = 0; i < LIGHT_NUM; ++i)
    {
        vec3 light_vec_dir = v_position - light_pos[i];
        float light_len = length(light_vec_dir);
        float attenuation = 1.0 / (light_attenuation[i] * light_len * light_len);

        vec3 light_vec_dirN = normalize(light_vec_dir);
        vec3 spor_dirN = normalize(spot_dir[i]);
        float cos_alpha = dot(light_vec_dirN, spor_dirN);
        float cos_half_theta = cos(spot_theta[i] / 2.0);
        float cos_half_phi = cos(spot_phi[i] / 2.0);
        if (cos_alpha <= cos_half_phi)
        {
            // out-range
            // attenuation * 0.f;
            ambient_total += ambient_color[i];
        }
        else
        {
            if (cos_alpha > cos_half_theta)
            {
                // inner corn
                // attenuation * 1.f
            }
            else
            {
                // outer corn
                attenuation *= pow((cos_alpha - cos_half_phi)/(cos_half_theta - cos_half_phi), spot_falloff[i]);
            }
            vec3 normal = normalize((vec4(v_normal, 0.0) * model_mat).xyz);
            vec3 light = -light_vec_dirN;
            float diffuse_power = clamp(dot(normal, light), 0.0, 1.0);
            vec3 eye = -normalize(eye_dir);
            vec3 half_vec = normalize(light + eye);
            float specular = pow(clamp(dot(normal, half_vec), 0.0, 1.0), specular_shininess[i]);

            diffuse_total += diffuse_color[i] * diffuse_power * attenuation;
            ambient_total += ambient_color[i];
            specular_total += specular_color[i] * specular;
        }
        frag_color = v_color * diffuse_total + ambient_total + specular_total + emissive_color;
    }
}
