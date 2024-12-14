#version 460 core

out vec4 FragColor;

uniform sampler2D oldTexture;
uniform sampler2D newTexture;
uniform float numRenderedFrame;
//uniform float delay;
uniform uvec2 viewSize;

in vec2 texCoord;

void main()
{
	vec2 TexCoords = gl_FragCoord.xy / vec2(viewSize);
	vec4 oldTexColor = texture(oldTexture, TexCoords);
	vec4 newTexColor = texture(newTexture, TexCoords);
	
	FragColor = mix(oldTexColor, newTexColor, 1 / float(numRenderedFrame + 1));
}