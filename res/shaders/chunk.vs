#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in int voxel_id;
layout(location = 2) in int face_id;

const vec2 uv_coords[4] = vec2[4](
    vec2(0, 0), vec2(0, 1), 
    vec2(1, 0), vec2(1, 1)
);

const int uv_indicecs[12] = int[12](
    1, 0, 2, 1, 2, 3, // even face
    3, 0, 2, 3, 1, 0 // odd face
);

out vec2 tex_Coord;
out float randomNum;

uniform mat4 model;
uniform mat4 project;
uniform mat4 view;

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{
    int uv_index = gl_VertexID % 6 + (face_id & 1) * 6;
    tex_Coord = uv_coords[uv_indicecs[uv_index]];
    randomNum = rand(vec2(aPos.x, aPos.z));
    gl_Position = project * view * model * vec4(aPos, 1.0);
}