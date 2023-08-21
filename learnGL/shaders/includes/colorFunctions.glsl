#ifndef COLOR_FUNCTIONS_GLSL
#define COLOR_FUNCTIONS_GLSL

vec4 gammaCorrect(vec4 color)
{
    return vec4(pow(color.rgb, vec3(1/2.2f)),color.a);
}
vec4 gammaCorrect(vec4 color,float gamma)
{
    return vec4(pow(color.rgb, vec3(1/gamma)),color.a);
}

vec4 unGammaCorrect(vec4 color)
{
    return vec4(pow(color.rgb, vec3(2.2f) ),color.a);
}
vec4 unGammaCorrect(vec4 color,float gamma)
{
    return vec4(pow(color.rgb, vec3(gamma) ),color.a);
}


// reinhard Tone mapping is one of the simplest tone mapping algorithms
vec4 ReihnardToneMapping (vec4 hdrColor)
{
  return vec4(hdrColor.rgb / (hdrColor.rgb+vec3(1.0f) ), hdrColor.a);
}

vec4 ExposureToneMapping(vec4 hdrColor, float exposure)
{
  vec3 toneMappedRGB = vec3(1.0f) - exp(-hdrColor.rgb * exposure);
  return  vec4(toneMappedRGB,hdrColor.a);
}

vec4 PostProcess(vec4 color, float exposure)
{
  vec4 toneMappedLDR = ExposureToneMapping(color,exposure);
  return gammaCorrect(toneMappedLDR,2.2f);

}
#endif
