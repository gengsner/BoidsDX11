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

struct TerrainVertexInputType
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent: TANGENT;
    float2 uv : TEXCOORD0;
    float4 color : COLOR;
};

struct TerrainPixeLInputType
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float4 worldPosition : POSITION0;
    float4 normal : NORMAL;
    float4 worldNormal : WORLDNORMAL;
    float2 uv : TEXCOORD0;
    float3x3 tBN : TBN;
    float clip : SV_ClipDistance0;
};


float3 s_curve(float3 x)
{
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

float3 tonemap_s_gamut3_cine(float3 c)
{
    // based on Sony's s gamut3 cine
    float3x3 fromSrgb = float3x3(
        +0.6456794776, +0.2591145470, +0.0952059754,
        +0.0875299915, +0.7596995626, +0.1527704459,
        +0.0369574199, +0.1292809048, +0.8337616753);

    float3x3 toSrgb = float3x3(
        +1.6269474099, -0.5401385388, -0.0868088707,
        -0.1785155272, +1.4179409274, -0.2394254004,
        +0.0444361150, -0.1959199662, +1.2403560812);

    return mul(toSrgb, s_curve(mul(fromSrgb, c)));
}

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