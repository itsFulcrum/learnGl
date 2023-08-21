#version 450 core
// vertexData
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;
layout (location=2) in vec3 aTangent;
layout (location=3) in vec3 aBitangent;
layout (location=4) in vec3 aColor;
layout (location=5) in vec2 aTexCoord;

uniform mat4 _modelMatrix;
uniform mat4 _normalMatrix;
uniform mat4 _viewMatrix;
uniform mat4 _projectionMatrix;

out vec2 uv;
out vec3 positionOS;
out vec3 positionWS;
out vec3 normalOS;
out vec3 normalWS;
out mat3 tangentToWorldMatrix;

void main()
{
	gl_Position = _projectionMatrix * _viewMatrix * _modelMatrix * vec4(aPos.xyz, 1.0);
	// outputs to fragment shader
	uv = aTexCoord;

	positionOS = aPos;
	positionWS = vec3(_modelMatrix * vec4(aPos.xyz, 1.0));

	//normalOS = aNormal;
	/*
	Inversing matrices is a costly operation for shaders, so wherever possible try to avoid doing inverse operations since they have to be done on each vertex of your scene.
	For learning its fine, but for an efficient application you'll want to calculate the normal matrix on the CPU
	and send it to the shaders via a uniform before drawing (just like the model matrix).*/
	normalOS = normalize(aNormal);
	normalWS = vec4(_normalMatrix * vec4(normalOS,0.0f)).xyz;

// try multiplying with normal matrix
	vec3 T = normalize(vec3(_normalMatrix * vec4(aTangent,   0.0)));
  vec3 B = normalize(vec3(_normalMatrix * vec4(aBitangent, 0.0)));
  vec3 N = normalize(vec3(_normalMatrix * vec4(aNormal,    0.0)));
  tangentToWorldMatrix = mat3(T, B, N);

}
