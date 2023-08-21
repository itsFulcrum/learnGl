#ifndef MATH_FUNCTIONS_GLSL
#define MATH_FUNCTIONS_GLSL

float saturate(float value)
{
  return clamp(value,0.0f,1.0f);
}

float clamp01(float value)
{
  return clamp(value,0.0f,1.0f);
}

float oneMinus(float value)
{
    return 1.0f-value;
}

float lerp(float a, float b, float t)
{
  return (1.0f - t) * a + b * t;
}
float inverseLerp(float a, float b, float v)
{
  return (v-a) / (b - a);
}
float remap(float iMin,float iMax,float oMin,float oMax, float v)
{
  float t = inverseLerp(iMin,iMax,v);
  return lerp(oMin,oMax,t);
}
vec3 lerp(vec3 a,vec3 b, float t)
{
    vec3 result = vec3(0.0f,0.0f,0.0f);
    result.x = lerp(a.x,b.x,t);
    result.y = lerp(a.y,b.y,t);
    result.z = lerp(a.z,b.z,t);

    return result;
}
vec4 lerp(vec4 a,vec4 b, float t)
{
    vec4 result = vec4(0.0f,0.0f,0.0f,0.0f);
    result.x = lerp(a.x,b.x,t);
    result.y = lerp(a.y,b.y,t);
    result.z = lerp(a.z,b.z,t);
    result.w = lerp(a.w,b.w,t);

    return result;
}

#endif
