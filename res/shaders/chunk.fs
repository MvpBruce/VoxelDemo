#version 330 core

out vec4 fragmentColor;

in vec2 tex_Coord;
in float randomNum;

uniform sampler2D texture;
//uniform sampler2D texture2;
void main()
{
    vec3 texCol = texture(texture, tex_Coord).rgb;\
    fragmentColor = vec4(texCol, 1.0);
   // fragmentColor = vec4(vec3(1.0, randomNum, randomNum), 1.0);
}