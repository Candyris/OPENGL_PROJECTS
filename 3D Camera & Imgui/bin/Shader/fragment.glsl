#version 330 core
out vec4 color;

in vec4 ourColor;
in vec2 TextCoord;

uniform sampler2D text;
uniform vec3 u_color;

void main()
{
    vec4 mixedColor = mix(vec4(u_color, 1.0f), ourColor, 0.5);
    color = texture(text,TextCoord)* (mixedColor) ;
}
