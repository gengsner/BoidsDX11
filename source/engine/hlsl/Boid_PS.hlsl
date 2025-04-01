#include "Common.hlsli"
#include "FrameBuffer.hlsli"

PixelOutput main(PixelInputType input)
{
    PixelOutput result;    
    
    float dLIntensity = max(0, dot(lightDir, input.worldNormal.xyz)) * dirLightIntensity;
    
    float3 dLColor = dirLightColor * dLIntensity;
    float3 aColor =  ambientLightColor * ambientLightIntensity;
         
    float dist = min(fogDist, length(camPos - input.worldPosition.xyz));
    
    float fog = (fogDist - dist) / fogDist;
    
    result.color.rgb = fog * (input.color.rgb * dLColor + input.color.rgb * aColor);
    result.color.rgb += (1 - fog) * (fogColor);
           
    result.color.a = 1.f;
    return result;
}