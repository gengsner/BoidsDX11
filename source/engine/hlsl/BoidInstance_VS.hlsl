#include "FrameBuffer.hlsli"
#include "ObjectBuffer.hlsli"
#include "Common.hlsli"
#include "BoidCommon.hlsli"

PixelInputType main(VertexInputType input)
{        
    Boid b = boids[input.instanceID];
        
    float3 forward = normalize(b.vel);
    float3 up = float3(0, 1, 0);
    float3 right = normalize(cross(forward, up));
    up = cross(right, forward);
    
    float4x4 instanceModelToWorld =
    {
        right.x, up.x, forward.x, b.pos.x,
        right.y, up.y, forward.y, b.pos.y,
        right.z, up.z, forward.z, b.pos.z,
        0, 0, 0, 1
    };
    
    PixelInputType output;
    
    float4 vertexObjectPos = float4(input.position, 1.0);
    float4 vertexObjectNormal = float4(input.normal, 0.0);

    float4 vertexWorldPos = mul(instanceModelToWorld, vertexObjectPos);
    float4 vertexClipPos = mul(worldToClipMatrix, vertexWorldPos);
    float4 vertexWorldNormal = mul(instanceModelToWorld, vertexObjectNormal);
    
    output.position = vertexClipPos;
    output.worldPosition = vertexWorldPos;

    float mainColor = 1.f;
    float altColor = 0.f;
    if (flockSizeToFullyColor > 0)
    {
        altColor = min(1.f, ((float) b.flockSize / (float) flockSizeToFullyColor));
        mainColor = 1.f - altColor;                
    }
    output.color = float4(boidColor * mainColor + boidAltColor * altColor , 1.0);
    output.worldNormal = vertexWorldNormal;
    output.uv = input.uv;
    output.normal = vertexObjectNormal;
    
    return output;
}