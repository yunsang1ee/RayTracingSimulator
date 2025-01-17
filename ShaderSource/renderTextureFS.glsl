#version 460 core

out vec4 FragColor;

uniform sampler2D textureSampler;
uniform vec2 viewSize;

void main()
{
	vec2 texCoords = gl_FragCoord.xy / viewSize;
	FragColor = texture(textureSampler, texCoords);
}