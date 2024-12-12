#version 460 core

out vec4 FragColor;

uniform sampler2D frontTexture;
uniform sampler2D backTexture;
uniform float numRenderedFrame;
uniform vec2 viewSize;

void main()
{
	vec2 TexCoords = gl_FragCoord.xy / viewSize;
	vec4 frontTexColor = texture(frontTexture, TexCoords);
	vec4 backTexColor = texture(backTexture, TexCoords);
	
	FragColor = mix(frontTexColor, backTexColor, 1 / numRenderedFrame);
}