#version 330 core

layout(location = 0) out vec4 fragmentColor;

in vec3 selectorColor;
in vec2 tex_Coord;

uniform sampler2D u_texture;

void main()
{
    fragmentColor = texture(u_texture, tex_Coord);
    fragmentColor.rgb += selectorColor;
    if (fragmentColor.r + fragmentColor.b > 1.0)
    {
        discard;
    }
}