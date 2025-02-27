#version 330 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec2 in_tex_coord;

uniform mat4 model;
uniform mat4 project;
uniform mat4 view;
uniform int nOperation;

const vec3 selectorColors[2] = vec3[2](vec3(1.0, 0.0, 0.0), vec3(0.0, 0.0, 1.0));

out vec3 selectorColor;
out vec2 tex_Coord;

void main()
{
    tex_Coord = in_tex_coord;
    selectorColor = selectorColors[nOperation];
    gl_Position = project * view * model * vec4((in_position - 0.5) * 1.01 + 0.5, 1.0);
}