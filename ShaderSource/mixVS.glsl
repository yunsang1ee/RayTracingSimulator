#version 460 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTex;
layout(location = 2) in vec3 inNormal;

out vec2 texCoord;

void main()
{
	texCoord = inTex;
	gl_Position = vec4(inPosition, 1.0f);
}