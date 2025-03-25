#include "FrameBuffer.hlsli"
#include "ObjectBuffer.hlsli"
#include "Common.hlsli"

PixelInputType main(VertexInputType input)
{
    PixelInputType output;    
    float4 vertexObjectPos = float4(input.position, 1.0);
    float4 vertexObjectNormal = float4(input.normal, 0.0);
    float4 vertexWorldPos = mul(modelToWorld, vertexObjectPos);
    float4 vertexClipPos = mul(worldToClipMatrix, vertexWorldPos);    
    float4 vertexWorldNormal = mul(modelToWorld, vertexObjectNormal);
    
    output.position = vertexClipPos;
    output.worldPosition = vertexWorldPos;
    output.color = float4(boundsColor, 1.0);
    output.worldNormal = float4(normalize(vertexWorldNormal.xyz), 0.f);
    output.uv = input.uv;
    output.normal = vertexObjectNormal;
        
    return output;    
}