/*
 * Portions of this code are from Microsoft's System.Numerics.
 * Link: https://github.com/microsoft/referencesource/blob/master/System.Numerics/System/Numerics/Quaternion.cs#L289
 * Copyright (c) Microsoft Corporation.
 * Licensed under the MIT License.
 */

#pragma once
#include "Vector4.h"
#include "Matrix3x3.h"

namespace CommonUtilities
{
	template <class T>
	class Quaternion
	{
	public:
		Quaternion<T>();
		Quaternion<T>(const T& aW, const T& aX, const T& aY, const T& aZ);
		Quaternion<T>(const T& aYaw, const T& aPitch, const T& aRoll);

		inline Matrix3x3<T> GetRotationMatrix3x3() const;
		inline Matrix4x4<T> GetRotationMatrix4x4() const;
		void SLerp(const Quaternion<T>& aQuaternion, const float aFraction)
		{
			const float epsilon = 1e-6f;

			float t = min(1.f, aFraction);

			float cosOmega = myX * aQuaternion.myX + myY * aQuaternion.myY +
				myZ * aQuaternion.myZ + myW * aQuaternion.myW;

			bool flip = false;

			if (cosOmega < 0.0f)
			{
				flip = true;
				cosOmega = -cosOmega;
			}

			float s1, s2;

			if (cosOmega > (1.0f - epsilon))
			{
				s1 = 1.0f - t;
				s2 = (flip) ? -t : t;
			}
			else
			{
				float omega = acos(cosOmega);
				float invSinOmega = (1.f / sin(omega));

				s1 = sin((1.0f - t) * omega) * invSinOmega;
				s2 = (flip)
					? -sin(t * omega) * invSinOmega
					: sin(t * omega) * invSinOmega;
			}

			myX = s1 * myX + s2 * aQuaternion.myX;
			myY = s1 * myY + s2 * aQuaternion.myY;
			myZ = s1 * myZ + s2 * aQuaternion.myZ;
			myW = s1 * myW + s2 * aQuaternion.myW;
		}
		Quaternion<T>& operator*=(const Quaternion<T>& aQuaternion)
		{
			float q1x = myX;
			float q1y = myY;
			float q1z = myZ;
			float q1w = myW;

			float q2x = aQuaternion.myX;
			float q2y = aQuaternion.myY;
			float q2z = aQuaternion.myZ;
			float q2w = aQuaternion.myW;

			float cx = q1y * q2z - q1z * q2y;
			float cy = q1z * q2x - q1x * q2z;
			float cz = q1x * q2y - q1y * q2x;

			float dot = q1x * q2x + q1y * q2y + q1z * q2z;

			myX = q1x * q2w + q2x * q1w + cx;
			myY = q1y * q2w + q2y * q1w + cy;
			myZ = q1z * q2w + q2z * q1w + cz;
			myW = q1w * q2w - dot;

			return *this;
		}

		float myW;
		float myX;
		float myY;
		float myZ;

	private:
	};

	template<class T>
	inline Quaternion<T>::Quaternion()
	{
		myW = static_cast<T>(1);
		myX = static_cast<T>(0);
		myY = static_cast<T>(0);
		myZ = static_cast<T>(0);
	}

	template<class T>
	inline Quaternion<T>::Quaternion(const T& aW, const T& aX, const T& aY, const T& aZ)
	{
		myW = aW;
		myX = aX;
		myY = aY;
		myZ = aZ;
	}

	template<class T>
	inline Quaternion<T>::Quaternion(const T& aYaw, const T& aPitch, const T& aRoll)
	{
		float sr, cr, sp, cp, sy, cy;

		float halfRoll = aRoll * 0.5f;
		sr = sin(halfRoll);
		cr = cos(halfRoll);

		float halfPitch = aPitch * 0.5f;
		sp = sin(halfPitch);
		cp = cos(halfPitch);

		float halfYaw = aYaw * 0.5f;
		sy = sin(halfYaw);
		cy = cos(halfYaw);

		myX = cy * sp * cr + sy * cp * sr;
		myY = sy * cp * cr - cy * sp * sr;
		myZ = cy * cp * sr - sy * sp * cr;
		myW = cy * cp * cr + sy * sp * sr;
	}

	template<class T>
	inline Matrix3x3<T> Quaternion<T>::GetRotationMatrix3x3() const
	{
		Matrix3x3<T> result;

		float xx = myX * myX;
		float yy = myY * myY;
		float zz = myZ * myZ;

		float xy = myX * myY;
		float wz = myZ * myW;
		float xz = myZ * myX;
		float wy = myY * myW;
		float yz = myY * myZ;
		float wx = myX * myW;

		result(1, 1) = 1.0f - 2.0f * (yy + zz);
		result(1, 2) = 2.0f * (xy + wz);
		result(1, 3) = 2.0f * (xz - wy);
		result(1, 4) = 0.0f;
		result(2, 1) = 2.0f * (xy - wz);
		result(2, 2) = 1.0f - 2.0f * (zz + xx);
		result(2, 3) = 2.0f * (yz + wx);
		result(2, 4) = 0.0f;
		result(3, 1) = 2.0f * (xz + wy);
		result(3, 2) = 2.0f * (yz - wx);
		result(3, 3) = 1.0f - 2.0f * (yy + xx);
		result(3, 4) = 0.0f;

		return result;
	}

	template<class T>
	inline Matrix4x4<T> Quaternion<T>::GetRotationMatrix4x4() const
	{
		Matrix4x4<T> result;

		float xx = myX * myX;
		float yy = myY * myY;
		float zz = myZ * myZ;

		float xy = myX * myY;
		float wz = myZ * myW;
		float xz = myZ * myX;
		float wy = myY * myW;
		float yz = myY * myZ;
		float wx = myX * myW;

		result(1, 1) = 1.0f - 2.0f * (yy + zz);
		result(1, 2) = 2.0f * (xy + wz);
		result(1, 3) = 2.0f * (xz - wy);
		result(1, 4) = 0.0f;
		result(2, 1) = 2.0f * (xy - wz);
		result(2, 2) = 1.0f - 2.0f * (zz + xx);
		result(2, 3) = 2.0f * (yz + wx);
		result(2, 4) = 0.0f;
		result(3, 1) = 2.0f * (xz + wy);
		result(3, 2) = 2.0f * (yz - wx);
		result(3, 3) = 1.0f - 2.0f * (yy + xx);
		result(3, 4) = 0.0f;
		result(4, 1) = 0.0f;
		result(4, 2) = 0.0f;
		result(4, 3) = 0.0f;
		result(4, 4) = 1.0f;

		return result;
	}
}

