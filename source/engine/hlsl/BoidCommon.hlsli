#include "ComputeShaderDefines.h"

struct Boid
{
    float3 pos;
    uint cellIndex;
    float3 vel;
    uint flockSize;
};

#define groupSize THREAD_GROUP_SIZE
#define doubleGroupSize DOUBLE_THREAD_GROUP_SIZE
#define maxBoids 10000000

RWStructuredBuffer<Boid> boidsIn : register(u0);
RWStructuredBuffer<Boid> boidsOut : register(u1);

RWStructuredBuffer<uint> sumBuffer : register(u2);
RWStructuredBuffer<uint> unsortedSumBuffer : register(u3);

cbuffer sortingStageBuffer : register(b1)
{
    uint2 groupIteration;
    uint2 groupIterationPadding;
};

StructuredBuffer<Boid> boids : register(t0);