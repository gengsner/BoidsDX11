#include "Camera.h"
#include "commonUtilities/UtilityFunctions.h"

constexpr float MOVE_SPEED{ 50.f };
constexpr float ROT_SPEED{ 0.1f };
constexpr float WHEEL_SPEED{ 0.2f };
constexpr float NEARP{ 0.001f };
constexpr float FARP{ 10000.0f };
constexpr double DEGREE_RADIAN_RATIO{ 3.14159265358979323846 / 180.0 };

Camera::Camera(const CommonUtilities::Matrix4x4<float>& aTransform) :
	myTransform(aTransform),
	myProjectionMatrix(),
	myResolution()
{
	myMoveState = eMove::Key;
}

void Camera::Init(const Vector2<float>& aResolution, const float& aFoVDegrees, const CommonUtilities::Vector3<float>& aPosition)
{
	float radians{ aFoVDegrees * (float)DEGREE_RADIAN_RATIO };

	myProjectionMatrix = { 1.0f / tan(radians / 2.0f), 0, 0, 0,
							0, 1.0f / tan(radians / 2.0f), 0, 0,
							0, 0, 1.0f / FARP, 1.0f,
							0, 0, -NEARP / FARP, 0 };

	SetResolution(aResolution);
	myTransform.SetTranslation(aPosition);
}

void Camera::SetResolution(const Vector2<float>& aResolution)
{
	myResolution = aResolution;
	myProjectionMatrix(2, 2) *= (myResolution.x / myResolution.y);
}

void Camera::Update(float aDeltaTime, CommonUtilities::InputHandler& aInputHandler)
{
	if (aInputHandler.GetKeyDown(VK_MBUTTON))
	{
		myMoveState = eMove::MMB;
	}
	else if (aInputHandler.GetKeyDown(VK_RBUTTON))
	{
		myMoveState = eMove::RMB;
	}
	else if ((aInputHandler.GetKeyRelease(VK_MBUTTON) && myMoveState == eMove::MMB) ||
		(aInputHandler.GetKeyRelease(VK_RBUTTON) && myMoveState == eMove::RMB))
	{
		myMoveState = eMove::Key;
	}


	int wheelDelta = aInputHandler.GetMouseWheelDelta();
	if (wheelDelta != 0)
	{
		myMoveSpeedFactor += wheelDelta * WHEEL_SPEED;
		myMoveSpeedFactor = Clamp(myMoveSpeedFactor, 0.01f, 100.f);
	}

	myMoveSpeed = MOVE_SPEED * myMoveSpeedFactor;

	if (aInputHandler.GetKey(VK_SHIFT))
	{
		myMoveSpeed *= 2.5f;
	}

	CommonUtilities::Vector3<float> moveDelta;
	CommonUtilities::Vector2<float> rotDelta;
	auto p = aInputHandler.GetMouseDeltaPos();

	switch (myMoveState)
	{
	case(eMove::MMB):
	{
		moveDelta.x -= p.x * myMoveSpeed * 5.f;
		moveDelta.y += p.y * myMoveSpeed * 5.f;
		break;

	}
	case(eMove::RMB):
	{
		rotDelta.y -= p.x * ROT_SPEED;
		rotDelta.x -= p.y * ROT_SPEED;
		break;
	}
	}

	if (aInputHandler.GetKey('W')) ++moveDelta.z;
	if (aInputHandler.GetKey('S')) --moveDelta.z;
	if (aInputHandler.GetKey('A')) --moveDelta.x;
	if (aInputHandler.GetKey('D')) ++moveDelta.x;
	if (aInputHandler.GetKey(VK_SPACE)) ++moveDelta.y;
	if (aInputHandler.GetKey(VK_CONTROL)) --moveDelta.y;
	if (aInputHandler.GetKey('Q')) rotDelta.y -= aDeltaTime;
	if (aInputHandler.GetKey('E')) rotDelta.y += aDeltaTime;

	if (moveDelta.Length() > myMoveSpeed)
		moveDelta = moveDelta.GetNormalized() * myMoveSpeed;
	MoveCamera(moveDelta, aDeltaTime);
	RotateCamera(rotDelta);
}

void Camera::MoveCamera(const CommonUtilities::Vector3<float>& aDelta, const float aDeltaTime)
{
	Vector3<float> delta;
	delta += myTransform.GetX() * myMoveSpeed * aDeltaTime * aDelta.x;
	delta += myTransform.GetY() * myMoveSpeed * aDeltaTime * aDelta.y;
	delta += myTransform.GetZ() * myMoveSpeed * aDeltaTime * aDelta.z;
	myTransform.SetTranslation(myTransform.GetTranslation() + delta);
}

void Camera::RotateCamera(const CommonUtilities::Vector2<float>& aDelta)
{
	myYaw -= aDelta.y;
	myPitch += aDelta.x;
	myPitch = Clamp(myPitch, -89.f, 89.f);

	float pitch = myPitch * (float)DEGREE_RADIAN_RATIO;
	float yaw = myYaw * (float)DEGREE_RADIAN_RATIO;

	Vector3<float> forward;
	forward.x = sin(yaw) * cos(pitch);
	forward.y = sin(pitch);
	forward.z = cos(yaw) * cos(pitch);
	forward.Normalize();

	Vector3<float> up{ 0.f, 1.f, 0.f };
	Vector3<float> right = up.Cross(forward).GetNormalized();
	up = forward.Cross(right);

	myTransform.SetX(right);
	myTransform.SetY(up);
	myTransform.SetZ(forward);
}

void Camera::SetTransform(const Matrix4x4<float>& aTransform)
{
	myTransform = aTransform;
}

Vector3<float> Camera::GetViewDir()
{
	return { myTransform(3,1), myTransform(3,2), myTransform(3,3) };
}

Vector3<float> Camera::GetPos()
{
	return { myTransform(4,1), myTransform(4,2), myTransform(4,3) };

}

Vector4<float> Camera::GetPostProjectionPos(const Vector3<float>& aWorldPos) const
{
	Matrix4x4<float> inverseTransform{ myTransform.GetFastInverse(myTransform) };

	Vector4<float> w{ aWorldPos.x, aWorldPos.y, aWorldPos.z, 1.f };
	Vector4<float> p{ w * inverseTransform };

	p = p * myProjectionMatrix;

	p.x /= p.w;
	p.y /= p.w;

	return p;
}

const Matrix4x4<float> Camera::GetWorldToClipMatrix()
{
	return myTransform.GetFastInverse(myTransform) * myProjectionMatrix;
}
