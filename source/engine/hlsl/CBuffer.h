#pragma once
#include "CommonUtilities/Matrix4x4.h"
#include "CommonUtilities/Vector2.h"

using namespace CommonUtilities;

struct FrameBufferData
{
	Matrix4x4<float> worldToClipMatrix;
	
	Vector3<float> lightDir;
	float deltaTime;

	Vector3<float> dirLightColor;
	float dirLightIntensity;

	Vector3<float> ambientLightColor;
	float ambientLightIntensity;

	Vector3<float> boidColor;
	int neiborsToFullyColor;

	Vector3<float> boidAltColor;
	float maxSpeed;

	Vector3<float> fogColor;
	float fogDist;

	Vector3<float> boundsColor;
	float fieldOfViewPercent;

	Vector3<float> camPos;
	float minSpeed;

	Vector2<int> resolution;
	float visualRangeSqr;
	float protectedRangeSqr;

	float cohesionFactor;
	float separationFactor;
	float alignmentFactor;
	unsigned int boidCount;

	Vector3<float> minPos;
	float turnMargin;

	Vector3<float> maxPos;
	float turnSpeed;

	Vector3<float> playerPosition;
	float cellSize;

	Vector3<float> playerFuturePosition;
	unsigned int cellCount;

	Vector3<unsigned int> gridDims;
	float gravity;

	Vector3<float> playerColor;
	float playerAttraction;
};
struct ObjectBufferData
{
	Matrix4x4<float> modelToWorldMatrix;
};