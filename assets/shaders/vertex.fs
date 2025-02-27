#version 330 core

out vec4 fragmentColor;
in vec3 OutColor;
in vec2 tex_Coord;

uniform sampler2D texture1;
uniform sampler2D texture2;
void main()
{
    fragmentColor = mix(texture(texture1, tex_Coord), texture(texture2, tex_Coord), 0.2);
}