#version 450 core

in vec4 position;
in vec4 vertexColor;

out vec4 fragmentColor;


uniform float _sinTime;

void main()
{
	
	vec4 color = vec4(vertexColor.xyz, 1.0f) * _sinTime;
	fragmentColor = color; 
}