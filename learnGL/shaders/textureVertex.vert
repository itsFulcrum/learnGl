#version 450 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aColor;
layout (location=2) in vec2 aTexCoord;


out vec4 position;
out vec4 vertexColor;
out vec2 uv;

void main()
{
	gl_Position = vec4(aPos.xyz, 1.0);
	
	// outputs to fragment shader
	position = vec4(aPos.xyz, 1.0);
	vertexColor = vec4(aColor.rgb,1.0);
	uv = aTexCoord;
}