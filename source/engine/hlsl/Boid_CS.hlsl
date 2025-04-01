#include "FrameBuffer.hlsli"
#include "Common.hlsli"
#include "BoidCommon.hlsli"

void BoidBehaviors(inout Boid boid)
{
    float3 center = 0;
    float3 close = 0;
    float3 avgVel = 0;
    uint flockSize = 0;
    
    for (uint i = 0; i < boidCount; i++)
    {
        Boid other = boidsIn[i];
        float3 vecTo = other.pos - boid.pos;
        if (fieldOfViewPercent < (dot(normalize(vecTo), normalize(boid.vel)) + 1.f) * 0.5f)
        {
            continue;
        }
        float distSq = dot(vecTo, vecTo);
        if (distSq > 0 && distSq < visualRangeSqr)
        {
            if (distSq < protectedRangeSqr)
            {
                close -= vecTo / distSq;
            }
            center += other.pos;
            avgVel += other.vel;
            flockSize++;
        }
    }
    
    if (flockSize > 0)
    {
        center /= flockSize;
        avgVel /= flockSize;

        boid.vel += (center - boid.pos) * cohesionFactor * deltaTime;
        boid.vel += (avgVel - boid.vel) * alignmentFactor * deltaTime;
    }

    boid.flockSize = flockSize;
    boid.vel += close * separationFactor * deltaTime;
}

void BoidBehaviorsGridded(inout Boid boid)
{
    float3 center = 0;
    float3 close = 0;
    float3 avgVel = 0;
    uint flockSize = 0;
    int cell = boid.cellIndex;
    
    int yStep = gridDims.x;
    int zStep = gridDims.x * gridDims.y;
    
    for (int z = -zStep; z <= zStep; z += zStep)
    {
        for (int y = -yStep; y <= yStep; y += yStep)
        {
            for (int x = -1; x <= 1; x++)
            {                              
                uint curr = (((cell + x) + y) + z);
                
                uint start = 0;
                if (curr > 0)
                {
                    start = sumBuffer[curr - 1];
                }
                uint end = sumBuffer[curr];
                               
                for (uint i = start; i < end; i++)
                {
                    Boid other = boidsIn[i];
                    float3 vecTo = other.pos - boid.pos;
                    if (fieldOfViewPercent < (dot(normalize(vecTo), normalize(boid.vel)) + 1.f) * 0.5f)
                    {
                        continue;
                    }
                    float distSqr = dot(vecTo, vecTo);
                    if (distSqr > 0 && distSqr < visualRangeSqr)
                    {
                        if (distSqr < protectedRangeSqr)
                        {
                            close -= vecTo / distSqr;
                        }
                        center += other.pos;
                        avgVel += other.vel;
                        flockSize++;
                    }
                }
            }
        }
    }
   
    if (flockSize > 0)
    {
        center /= flockSize;
        avgVel /= flockSize;

        boid.vel += (center - boid.pos) * cohesionFactor * deltaTime;
        boid.vel += (avgVel - boid.vel) * alignmentFactor * deltaTime;
    }

    boid.flockSize = flockSize;
    boid.vel += close * separationFactor * deltaTime;
}

void ClampVels(inout Boid boid)
{
    float speed = length(boid.vel);
    if (maxSpeed < speed)
    {
        boid.vel = normalize(boid.vel) * maxSpeed;
    }
    if (speed < minSpeed)
    {
        boid.vel = normalize(boid.vel) * minSpeed;
    }
}

void AvoidWallBehavior(inout Boid boid)
{
    float3 maxPosDiff = maxPos - boid.pos;
    float3 minPosDiff = boid.pos - minPos;
    
    if (maxPosDiff.x < turnMargin)
    {
        float scale = (turnMargin - maxPosDiff.x);
        boid.vel.x -= deltaTime * turnSpeed * scale;
    }
    if (maxPosDiff.y < turnMargin)
    {
        float scale = (turnMargin - maxPosDiff.y);
        boid.vel.y -= deltaTime * turnSpeed * scale;
    }
    if (maxPosDiff.z < turnMargin)
    {
        float scale = (turnMargin - maxPosDiff.z);
        boid.vel.z -= deltaTime * turnSpeed * scale;
    }
   
    if (minPosDiff.x < turnMargin)
    {
        float scale = (turnMargin - minPosDiff.x);
        boid.vel.x += deltaTime * turnSpeed * scale;
    }
    if (minPosDiff.y < turnMargin)
    {
        float scale = (turnMargin - minPosDiff.y);
        boid.vel.y += deltaTime * turnSpeed * scale;
    }
    if (minPosDiff.z < turnMargin)
    {
        float scale = (turnMargin - minPosDiff.z);
        boid.vel.z += deltaTime * turnSpeed * scale;
    }
}

void PlayerAttraction(inout Boid boid)
{
    float3 vecFrom = boid.pos - playerPosition;
    float3 playerTrajectory = playerFuturePosition - playerPosition;
        
    float fraction = max(0.f, dot(vecFrom, playerTrajectory) / dot(playerTrajectory, playerTrajectory));
    float3 closesPointOnTrajectory = playerPosition + playerTrajectory * min(1.f, fraction);
             
    float3 vecTo = closesPointOnTrajectory - boid.pos;
    float distSqr = dot(vecTo, vecTo);

    if (distSqr > 0 && distSqr < visualRangeSqr)
    {
        if (distSqr < protectedRangeSqr)
        {
            boid.vel -= vecTo / distSqr;
        }
        boid.vel += normalize(vecTo) * playerAttraction * deltaTime;
    }
}

[numthreads(groupSize, 1, 1)]
void main(uint3 threadID : SV_DispatchThreadID)
{
    if (boidCount <= threadID.x)
    {
        return;
    }
    Boid b = boidsIn[threadID.x];
    BoidBehaviors(b);
    AvoidWallBehavior(b);
    if(playerAttraction != 0.f)
        PlayerAttraction(b);
    ClampVels(b);
    
    b.vel -= float3(0, gravity, 0);   
    b.pos += b.vel * deltaTime;
    
    boidsOut[threadID.x] = b;
}

[numthreads(groupSize, 1, 1)]
void mainGridded(uint3 threadID : SV_DispatchThreadID)
{
    if (boidCount <= threadID.x)
    {
        return;
    }
    Boid b = boidsIn[threadID.x];
    BoidBehaviorsGridded(b);
    AvoidWallBehavior(b);
    if (playerAttraction != 0.f)
        PlayerAttraction(b);
    ClampVels(b);
    
    b.vel -= float3(0, gravity, 0);
    b.pos += b.vel * deltaTime;
    
    boidsOut[threadID.x] = b;
}

[numthreads(groupSize, 1, 1)]
void init(uint3 threadID : SV_DispatchThreadID)
{
    if (threadID.x >= maxBoids)
    {
        return;
    }
        
    uint seed = threadID.x;
    float3 frac = float3(randomFloat(seed),
                        randomFloat(seed),
                        randomFloat(seed));
    
    float3 frac2 = float3(randomFloat(seed),
                        randomFloat(seed),
                        randomFloat(seed));
    
    float3 size = maxPos - minPos;
    
    float3 pos = float3(minPos.x + size.x * frac.x,
                        minPos.y + size.y * frac.y,
                        minPos.z + size.z * frac.z);

    float3 vel = frac2 - float3(0.5f, 0.5f, 0.5f);
    
    boidsIn[threadID.x].pos = pos;
    boidsIn[threadID.x].vel = vel;
    boidsOut[threadID.x].pos = pos;
    boidsOut[threadID.x].vel = vel;
}