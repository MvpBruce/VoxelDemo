#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

out vec3 OutColor;
out vec2 tex_Coord;

uniform mat4 model;
uniform mat4 project;
uniform mat4 view;
void main()
{
    gl_Position = project * view * model * vec4(aPos, 1.0);
    OutColor = aColor;
    tex_Coord = aTexCoord;
}