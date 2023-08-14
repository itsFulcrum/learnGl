#version 450 core

in vec4 position;
in vec4 vertexColor;
in vec2 uv;

uniform sampler2D colorMap;
uniform sampler2D textureMask;

uniform float mixValue;

out vec4 fragmentColor;

// using third party "HASHTAG"include parser
#include includes/mathFunctions.glsl


void main()
{
	float mixFactor = clamp01(mixValue);
	vec2 flippedUV = vec2(uv.x,uv.y);
	vec4 color = vec4(vertexColor.xyz, 1.0f);
	vec4 colorTexture = texture(colorMap,flippedUV);
	vec4 wallpaper = texture(textureMask,uv);
	fragmentColor = lerp(colorTexture,wallpaper,mixFactor);
}
