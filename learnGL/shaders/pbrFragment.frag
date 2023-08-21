#version 450 core
// using third party "HASHTAG"include parser
#include includes/mathFunctions.glsl
#include includes/pbrLightingTerms.glsl
#include includes/colorFunctions.glsl


in vec2 uv;
in vec3 positionOS;
in vec3 positionWS;
in vec3 normalOS;
in vec3 normalWS;
in mat3 tangentToWorldMatrix;

// scene inputs
uniform vec3 _lightColor;
uniform float _lightStrength;
uniform vec3 _lightPositionWS;
uniform vec3 _cameraPositionWS;

//pbr maps inputs
uniform sampler2D _albedoMap;
uniform sampler2D _roughnessMap;
uniform sampler2D _metallicMap;
uniform sampler2D _normalMap;
uniform sampler2D _aoMap;
uniform sampler2D _emissionMap;



vec3 lightDirection;
vec3 viewDirection;

out vec4 fragmentOutputColor;

void main()
{
	vec3 normal_OS = normalize(normalOS);
  vec3 normal_WS = normalize(normalWS);

	// if we use point light we would calc light direction here like this. if its a directional light we would set it via a uniform.

	viewDirection = normalize(positionWS - _cameraPositionWS);

	vec4 albedoMap = texture(_albedoMap,uv);
	float alpha = albedoMap.a;
  vec3 normalMap = vec4( texture(_normalMap,uv)).xyz;
  normalMap = normalMap * 2 - 1; // convert 0 to 1 range to -1 to 1
  normal_WS = normalize(tangentToWorldMatrix * normalMap);

	vec3 albedo = albedoMap.rgb;
	float roughness = 0.4f;
	float metallic = 0.0f;
	float ambientOcclusion = 1.0f;

	vec3 lightColor = vec3(1.0f,1.0f,1.0f); // setting light color here for now but should be set from app via uniform
	float lightStrength = 50.0f;
	//
	vec3 outgoingRadiance = vec3(0.0f); // commonly denoted as Lo

	// loop over all non directional lightsources
	 outgoingRadiance += CookTorranceSpecularBRDF(positionWS, normal_WS, -viewDirection, _lightPositionWS, lightColor * lightStrength, albedo, metallic, roughness);


	vec3 ambient = vec3(0.03) * albedo * ambientOcclusion;
	vec4 outColorLinear =	vec4(ambient + outgoingRadiance,alpha);


	// could set gamma globally as well by
	// enabaling glEnable(GL_FRAMEBUFFER_SRGB); for the framebuffer
	// but this gives more control  // Gamma correction cann only happen at the very very end

	fragmentOutputColor = PostProcess(outColorLinear,1.1f);

}
