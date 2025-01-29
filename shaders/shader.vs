#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;
uniform vec2 timePos;

void main()
{
   gl_Position = vec4(aPos.xy + timePos, aPos.z, 1.0);
   ourColor = aPos * 0.5 + 0.5;
}
