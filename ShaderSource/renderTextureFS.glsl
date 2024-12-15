#version 460 core

out vec4 FragColor;

uniform sampler2D textureSampler;
uniform uvec2 viewSize;

void main()
{
	vec2 texCoords = gl_FragCoord.xy / vec2(viewSize);
	FragColor = texture(textureSampler, texCoords);
}