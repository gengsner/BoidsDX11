#pragma once
#include "Mesh.h"
#include "commonUtilities/Quaternion.h"
#include "commonUtilities/Matrix4x4.h"

using namespace CommonUtilities;

struct Player
{
	Mesh mesh;
	Matrix4x4<float> transform;
	Quaternion<float> quaternion;
	float velocity = 0.f;
	Vector3<float> rotVelocity;
	bool active = false;
	bool controlled = false;
};

struct FollowCamera
{
	Matrix4x4<float> transform;
	Quaternion<float> quaternion;
	Vector3<float> target;
	float velocity = 0.f;
	float acceleration = 50.f;
	float maxRange = 30.f;
	float rotSpeed = 0.1f;
};