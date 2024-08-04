#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTextCoord;

out vec2 TextCoord;
out vec4 ourColor ;

uniform mat4 u_transform;

void main()
{
    gl_Position = vec4(aPos.x,aPos.y,0.0f,1.0f) * u_transform;
    ourColor = vec4(aColor,1.0f);
    TextCoord = aTextCoord;
}