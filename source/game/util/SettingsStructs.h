#pragma once
#include "CommonUtilities/Vector3.h"

using namespace CommonUtilities;
constexpr float halfSize = 300.f;

struct SimulationSettings
{
	int boidCount = 500000;
	bool griddingOn = true;
	float cellSizeMult = 1.f;
	float gravity = 0.f;

	float visualRange = 10.f;
	float protectedRange = 4.5f;
	float fieldOfView = 270.f;
	float cohesionFactor = 2.f;
	float separationFactor = 1.f;
	float alignmentFactor = 5.f;
	float maxSpeed = 21.f;
	float minSpeed = 14.f;
	float turnSpeed = 3.2f;
	float turnMagin = 5.f;

	Vector3<float> minPos = { -halfSize * 2.f, -halfSize, -halfSize };
	Vector3<float> maxPos = { halfSize * 2.f, halfSize, halfSize };
};

struct PlayerSettings
{
	float maxVelocity = 150.f;
	float acceleration = 300.f;
	float rollAcceleration = 0.09f;
	float mouseAcceleration = 0.03f;
	float boidAttraction = -100.f;
};

struct DirectionalLight
{
	Vector3<float> dir{ 0,-1,0 };
	Vector3<float> color{ 1,1,1 };
	float intensity = 0.5f;
};

struct AmbientLight
{
	Vector3<float> color{ 1,1,1 };
	float intensity = 0.5f;
};

struct GraphicsSettings
{
	bool renderBounds = true;
	DirectionalLight dirLight;
	AmbientLight ambientLight;
	Vector3<float> boidColor{ 0.1f,0.8f,0.1f };
	Vector3<float> boidAltColor{ 0.1f,0.8f,0.1f };
	int flockSizeToFullyColor = 100;
	Vector3<float> boundsColor{ 0.1f,0.1f,0.4f };
	Vector3<float> clearColor{ 0.2f, 0.2f, 0.2f };
	Vector3<float> playerColor{ 0.5f, 0.1f, 0.5f };
	float viewDist{ 10000.f };
};