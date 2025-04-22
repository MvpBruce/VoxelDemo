#version 330 core
layout (location = 0) out vec4 fragmentColor;

const vec3 gamma = vec3(2.2);
const vec3 inv_gamma = 1 / gamma;

in vec2 tex_Coord;
in vec3 voxel_color;

flat in int face_id_out;
flat in int voxel_id_out;

uniform sampler2DArray textureArray;

void main()
{
    vec2 face_uv = tex_Coord;
    face_uv.x = tex_Coord.x / 3.0 - min(face_id_out, 2)/ 3.0;

    vec3 tex_col = texture(textureArray, vec3(face_uv, voxel_id_out)).rgb;
    tex_col = pow(tex_col, gamma);
    //tex_col.rgb *= voxel_color;
    tex_col = pow(tex_col, inv_gamma);
    fragmentColor = vec4(tex_col, 1.0);
}