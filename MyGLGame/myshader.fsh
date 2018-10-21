#version 410

in vec3 v_position;
in vec3 v_normal;
in vec4 v_color;
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
layout (location=0) out vec4 frag_color;

void main()
{

    vec3 light_vec_dir = v_position - light_pos;
    float light_len = length(light_vec_dir);
    float attenuation = 1.0 / (light_attenuation * light_len * light_len);

    vec3 light_vec_dirN = normalize(light_vec_dir);
    vec3 spor_dirN = normalize(spot_dir);
    float cos_alpha = dot(light_vec_dirN, spor_dirN);
    float cos_half_theta = cos(spot_theta / 2.0);
    float cos_half_phi = cos(spot_phi / 2.0);
    if (cos_alpha <= cos_half_phi)
    {
        // out-range
        // attenuation * 0.f;
        frag_color = ambient_color + emissive_color;
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
            attenuation *= pow((cos_alpha - cos_half_phi)/(cos_half_theta - cos_half_phi), spot_falloff);
        }
        vec3 normal = normalize((vec4(v_normal, 0.0) * model_mat).xyz);
        vec3 light = -light_vec_dirN;
        float diffuse_power = clamp(dot(normal, light), 0.0, 1.0);
        vec3 eye = -normalize(eye_dir);
        vec3 half_vec = normalize(light + eye);
        float specular = pow(clamp(dot(normal, half_vec), 0.0, 1.0), specular_shininess);
        frag_color = v_color * diffuse_color * diffuse_power * attenuation + ambient_color + specular_color * specular + emissive_color;
    }
}
