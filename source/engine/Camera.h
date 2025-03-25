#pragma once
#include "commonUtilities/Matrix3x3.h"
#include "commonUtilities/Vector2.h"
#include "commonUtilities/InputHandler.h"

using namespace CommonUtilities;

class Camera
{
	enum class eMove
	{
		Key,
		MMB,
		RMB
	};

public:
	Camera(const Matrix4x4<float>& aTransform = CommonUtilities::Matrix4x4<float>());
	void Init(const Vector2<float>& aResolution, const float& aFoVDegrees, const CommonUtilities::Vector3<float>& aPosition);
	void SetResolution(const Vector2<float>& aResolution);
	void Update(float aDeltaTime, CommonUtilities::InputHandler& aInputHandler);
	void MoveCamera(const CommonUtilities::Vector3<float>& aDelta, const float aDeltaTime);
	void RotateCamera(const CommonUtilities::Vector2<float>& aDelta);
	void SetTransform(const Matrix4x4<float>& aTransform);
	Vector3<float> GetViewDir();
	Vector3<float> GetPos();

	Vector4<float> GetPostProjectionPos(const Vector3<float>& aWorldPos) const;
	const Matrix4x4<float> GetWorldToClipMatrix();


private:
	Matrix4x4<float> myTransform;
	Matrix4x4<float> myProjectionMatrix;
	Vector2<float> myResolution;
	eMove myMoveState;
	float myMoveSpeedFactor = 1.f;
	float myMoveSpeed = 1.f;
	float myPitch = 0.f;
	float myYaw = 0.f;
};