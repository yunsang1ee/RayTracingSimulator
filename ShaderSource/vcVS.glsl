#version 460 core

uniform mat4 projectionTrans;
uniform mat4 viewTrans;
uniform mat4 worldTrans;
uniform vec3 constColor;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
out vec3 outColor;

void main()
{
	gl_Position =  projectionTrans * viewTrans * worldTrans * vec4(position, 1.0);
	if (color == vec3(0.0, 0.0, 0.0))
		outColor =  vec3(0.0, 0.0, 1.0);
	else
		outColor = color;
}