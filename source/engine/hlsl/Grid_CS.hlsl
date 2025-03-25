#include "FrameBuffer.hlsli"
#include "Common.hlsli"
#include "BoidCommon.hlsli"

uint3 getGridIndices(Boid boid)
{
    uint indexX = (uint) (max(0.f, boid.pos.x - minPos.x) / cellSize);
    uint indexY = (uint) (max(0.f, boid.pos.y - minPos.y) / cellSize);
    uint indexZ = (uint) (max(0.f, boid.pos.z - minPos.z) / cellSize);
    
    return uint3(min(indexX, gridDims.x - 1), min(indexY, gridDims.y - 1), min(indexZ, gridDims.z - 1));
}

uint getCellIndex(Boid boid)
{
    uint3 gridIndices = getGridIndices(boid);
    return (gridDims.x * gridDims.y * gridIndices.z) + (gridDims.x * gridIndices.y) + gridIndices.x;
}

[numthreads(groupSize, 1, 1)]
void main(uint3 threadID : SV_DispatchThreadID)
{
}

[numthreads(doubleGroupSize, 1, 1)]
void clear(uint3 threadID : SV_DispatchThreadID)
{
    uint index = threadID.x;
    
    countBuffer[index] = 0;
    countBufferTemp[index] = 0;
}

[numthreads(groupSize, 1, 1)]
void count(uint3 threadID : SV_DispatchThreadID)
{
    if (boidCount <= threadID.x)
    {
        return;
    }
    
    Boid b = boidsOut[threadID.x];
    
    uint cellIndex = getCellIndex(b);
    boidsOut[threadID.x].cellIndex = cellIndex;
    
    InterlockedAdd(countBuffer[cellIndex], 1);
}

[numthreads(1, 1, 1)]
void sum(uint3 threadID : SV_DispatchThreadID)
{
   countBufferTemp[0] = countBuffer[0];
    
    for (uint i = 1; i < cellCount; i++)
    {
        countBuffer[i] += countBuffer[i - 1];
        countBufferTemp[i] = countBuffer[i];
    }
}

groupshared uint temp[doubleGroupSize];
[numthreads(groupSize, 1, 1)]
void sweepPrefixSum(uint3 groupThreadID : SV_GroupThreadID, uint3 groupID : SV_GroupID)
{
    uint localStepSize = groupIteration.x;
    uint groupStepSize = groupIteration.y;
   
    uint groupThreadX = groupThreadID.x;
        
    uint aiGroup = groupThreadX;
    uint biGroup = groupThreadX + groupSize;
               
    uint groupOffset = groupID.x * groupStepSize;
      
    uint ai = groupOffset + (localStepSize * aiGroup) + (localStepSize - 1);
    uint bi = groupOffset + (localStepSize * biGroup) + (localStepSize - 1);
    
    if (ai < cellCount)
    {
        temp[aiGroup] = countBuffer[ai];
    }
    else
    {
        temp[aiGroup] = 0;
    }
    
    if (bi < cellCount)
    {
        temp[biGroup] = countBuffer[bi];
    }
    else
    {
        temp[biGroup] = 0;
    }
    
    int offset = 1;
    for (uint i = groupSize; i > 0; i >>= 1)
    {
        GroupMemoryBarrierWithGroupSync();
        if (groupThreadX < i)
        {
            int a = offset * (2 * groupThreadX + 1) - 1;
            int b = offset * (2 * groupThreadX + 2) - 1;
            temp[b] += temp[a];
        }
        offset *= 2;
    }
    
    for (uint j = 2; j < doubleGroupSize; j *= 2)
    {
        offset >>= 1;
        GroupMemoryBarrierWithGroupSync();
        if (groupThreadX < j-1)
        {
            int a = offset * (groupThreadX + 1) - 1;
            int b = offset * (groupThreadX + 1) + (offset / 2) - 1;
            temp[b] += temp[a];
        }
    }
    GroupMemoryBarrierWithGroupSync();
    
    if (ai < cellCount)
        countBuffer[ai] = temp[aiGroup];
    if (bi < cellCount)
        countBuffer[bi] = temp[biGroup];
}

[numthreads(doubleGroupSize - 1, 1, 1)]
void groupBlockSum(uint3 groupThreadID : SV_GroupThreadID, uint3 groupID : SV_GroupID)
{    
    uint localStepSize = groupIteration.x;
    uint groupStepSize = groupIteration.y;
    
    uint groupStartIndex = (groupStepSize + groupID.x * groupStepSize) - 1;
    
    uint index = groupStartIndex + (localStepSize * (groupThreadID.x + 1));
               
    if (index < cellCount)
    {
        countBuffer[index] += countBuffer[groupStartIndex];
    }    
}

[numthreads(doubleGroupSize, 1, 1)]
void blockSum(uint3 threadID : SV_DispatchThreadID)
{
    uint threadX = threadID.x;
    for (uint i = doubleGroupSize; i < cellCount; i += doubleGroupSize)
    {
        uint a = i + threadX;
        if (a < cellCount)
        {
            countBuffer[a] += countBuffer[i - 1];
        }
        DeviceMemoryBarrierWithGroupSync();       
    }
}

[numthreads(groupSize, 1, 1)]
void copy(uint3 threadID : SV_DispatchThreadID)
{
    countBufferTemp[threadID.x] = countBuffer[threadID.x];
}

[numthreads(groupSize, 1, 1)]
void sort(uint3 threadID : SV_DispatchThreadID)
{
    if (boidCount <= threadID.x)
    {
        return;
    }
    
    Boid b = boidsOut[threadID.x];
    int offset = 0;
    InterlockedAdd(countBufferTemp[b.cellIndex], -1, offset);
      
    boidsIn[offset - 1] = b;
}