#version 330 core

layout (location = 0) out vec4 fragmentColor;

const vec3 gamma = vec3(2.2);
const vec3 inv_gamma = 1 / gamma;

in vec2 tex_Coord;
in vec3 voxel_color;

uniform sampler2D texture1;
//uniform sampler2D texture2;
void main()
{
    vec3 tex_col = texture(texture1, tex_Coord).rgb;
    tex_col = pow(tex_col, gamma);
    tex_col.rgb *= voxel_color;
    tex_col = pow(tex_col, inv_gamma);
    fragmentColor = vec4(tex_col, 1.0);
}