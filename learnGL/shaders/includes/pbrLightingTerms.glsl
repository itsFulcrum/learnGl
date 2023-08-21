#ifndef PBR_LIGHTING_TERMS_GLSL
#define PBR_LIGHTING_TERMS_GLSL

const float PI = 3.14159265359;
const float dialectricF0 = 0.04f;

float diffuse(vec3 normal,vec3 lightDirection)
{
    return clamp01(dot(normal,-lightDirection));
}


// normal distribution function TrowbridgeReitzGGX
float NormalDistributionGGX(float NdotH, float roughness)
{
  // disney and epic concluded that squaring roughness looks more acurate
  float a = roughness*roughness;
  float a2 = a*a;
  //float NdotH = max(dot(N,H),0.0f);
  float NdotH2 = NdotH * NdotH;

  float num = a2;
  float denom = (NdotH * (a2 - 1.0f) + 1.0f);
  denom = PI * denom * denom;
  return num / denom;
}

// geometry
float GeometrySchlickGGX(float NdotV, float k)
{
  //float r = (roughness +1.0f);
  //float k = (r*r) / 8.0f;
  float nom   = NdotV;
  float denom = NdotV * (1.0 - k) + k;

  return nom / denom;
}

float GeometrySmith(float NdotV, float NdotL, float roughness)
{
    // there is no need to calculate k twice in the GeometrySchlickGGX function
    float r = (roughness +1.0f);
    float k = (r*r) / 8.0f;

    float ggx1 = GeometrySchlickGGX(NdotV, k);
    float ggx2 = GeometrySchlickGGX(NdotL, k);

    return ggx1 * ggx2;
}

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0f - F0) * pow(clamp01(1.0f - cosTheta),5.0f);
}

vec3 CookTorranceSpecularBRDF(vec3 worldPosition, vec3 normal, vec3 viewDirection,vec3 lightPosition, vec3 lightColor ,vec3 albedo,float metallic,float roughness)
{
    // the CookTorranceSpecularBRDF needs to run per lightsource to calculate the combined radiance (energy) that his a given point/fragment
    vec3 lightDirection = normalize( lightPosition - worldPosition);
    vec3 halfVector = normalize(viewDirection + lightDirection);

    // light distance should only apply for non directional lights
    // for directional light we should keep light attentuation at 1;
    float lightDistance = length(lightPosition - worldPosition);
    // light attentuation using inverse squre law which is phisically correct.
    // -> may want to use constant linea quadratic equation for more controll
    //	float attenuation = 1.0 / (constantValue + linearValue * distance + quadraticValue * (distance * distance));
    float lightAttentuation = 1.0f / (lightDistance * lightDistance);
    vec3 radiance = lightColor * lightAttentuation;


    float NdotL = max(dot(normal,lightDirection),0.0f);
    float NdotV = max(dot(normal,viewDirection),0.0f);
    float NdotH = max(dot(normal,halfVector),0.0f);
    float HdotV = max(dot(halfVector,viewDirection) , 0.0f);


    vec3 F0 = mix(vec3(dialectricF0),albedo,metallic);
    float cosTheta = HdotV;
    vec3 fresnel = FresnelSchlick(cosTheta ,F0);

    float normalDistribution = NormalDistributionGGX(NdotH,roughness);
    float geometry = GeometrySmith(NdotV,NdotL,roughness);

    // cook torrance brdf equation
    vec3 numerator = normalDistribution * geometry * fresnel;
    float denominator = 4.0f * NdotV * NdotL + 0.0001f;
    vec3 specular = numerator / denominator;

    // this part makes sure that light energy that gets refracted does not get reflected
    // thus keeping energy conservation.
    // as metallic surfaces only reflect and not refract light we enforce a value of 0
    // for refraction when metallic is 0
    vec3 specularReflection = fresnel; // commonly denoted as kS
    vec3 diffuseReflection = vec3(1.0f) - specularReflection; // commonly denoted as kD
    diffuseReflection = diffuseReflection * (1.0f - metallic);

    vec3 outgoingRadiance = (diffuseReflection * albedo / PI + specular) * radiance * NdotL;
    return outgoingRadiance;
}

#endif
