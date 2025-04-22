#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in int voxel_id;
layout (location = 3) in int face_id;

out vec2 tex_Coord;
out vec3 voxel_color;

flat out int voxel_id_out;
flat out int face_id_out;

uniform mat4 model;
uniform mat4 project;
uniform mat4 view;

vec3 hash31(float p) {
    vec3 p3 = fract(vec3(p * 21.2) * vec3(0.1031, 0.1030, 0.0973));
    p3 += dot(p3, p3.yzx + 33.33);
    return fract((p3.xxy + p3.yzz) * p3.zyx) + 0.05;
}

void main()
{
    voxel_id_out = voxel_id;
    face_id_out = face_id;
	tex_Coord = aTexCoord;
    //voxel_color = hash31(voxel_id);
    gl_Position = project * view * model * vec4(aPos, 1.0);
}