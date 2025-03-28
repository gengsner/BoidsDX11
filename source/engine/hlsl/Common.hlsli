SamplerState aSampler : register(s0);

struct VertexInputType
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    uint instanceID : SV_InstanceID;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float4 worldPosition : POSITION0;
    float4 normal : NORMAL;
    float4 worldNormal : WORLDNORMAL;
    float2 uv : TEXCOORD0;
};

struct PixelOutput
{
    float4 color : SV_TARGET;
};

uint xorshift(uint state)
{
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

float randomFloat(inout uint state)
{    
    state = xorshift(state);
    return float(state) / 4294967295.0;
}