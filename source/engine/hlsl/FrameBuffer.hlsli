cbuffer FrameBuffer : register(b0)
{
    float4x4 worldToClipMatrix;
	
	float3 lightDir;
    float deltaTime;

	float3 dirLightColor;
    float dirLightIntensity;

	float3 ambientLightColor;
    float ambientLightIntensity;

	float3 boidColor;
    int neiborsToFullyColor;

	float3 boidAltColor;
    float maxSpeed;

	float3 fogColor;
    float fogDist;

	float3  boundsColor;
    float fieldOfViewPercent;

	float3 camPos;
    float minSpeed;

	int2 resolution;
    float visualRangeSqr;
    float protectedRangeSqr;

    float cohesionFactor;
    float separationFactor;
    float alignmentFactor;
    uint boidCount;

	float3 minPos;
    float turnMargin;

	float3 maxPos;
    float turnSpeed;

	float3 playerPosition;
    float cellSize;

	float3 playerFuturePosition;
    uint cellCount;

	uint3 gridDims;
    float gravity;

	float3 playerColor;
    float playerAttraction;
}