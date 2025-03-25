#pragma once
#include "Plane.h"
#include "Ray.h"
#include "AABB3D.h"
#include "Sphere.h"
#include "UtilityFunctions.h"
#include <limits>

namespace CommonUtilities
{
	template <class T>
	bool IntersectionPlaneRay(const Plane<T>& aPlane, const Ray<T>& aRay, Vector3<T>& aOutIntersectionPoint)
	{
		T x = (aPlane.GetPoint() - aRay.GetOrigin()).Dot(aPlane.GetNormal());
		T y = aRay.GetDirection().Dot(aPlane.GetNormal());

		//special case - ray parallel to plane
		if (y == T{})
		{
			if (x == T{})
			{
				aOutIntersectionPoint = aRay.GetOrigin();
				return true;
			}
			else
			{
				return false;
			}
		}

		T t = x / y;

		//special case - ray going away from plane
		if (t < T{})
		{
			return false;
		}

		aOutIntersectionPoint = aRay.GetOrigin() + aRay.GetDirection() * t;
		return true;
	}

	template <class T>
	bool IntersectionAABBRay(const AABB3D<T>& aAABB, const Ray<T>& aRay, Vector3<T>& aOutIntersectionPoint)
	{
		Vector3<T> invDir;

		if (aRay.GetDirection().x == T{}) invDir.x = std::numeric_limits<T>::max();
		else invDir.x = static_cast<T>(1) / aRay.GetDirection().x;
		if (aRay.GetDirection().y == T{}) invDir.y = std::numeric_limits<T>::max();
		else invDir.y = static_cast<T>(1) / aRay.GetDirection().y;
		if (aRay.GetDirection().z == T{}) invDir.z = std::numeric_limits<T>::max();
		else invDir.z = static_cast<T>(1) / aRay.GetDirection().z;		

		T t1 = (aAABB.GetMin().x - aRay.GetOrigin().x) * invDir.x;
		T t2 = (aAABB.GetMax().x - aRay.GetOrigin().x) * invDir.x;
		T t3 = (aAABB.GetMin().y - aRay.GetOrigin().y) * invDir.y;
		T t4 = (aAABB.GetMax().y - aRay.GetOrigin().y) * invDir.y;
		T t5 = (aAABB.GetMin().z - aRay.GetOrigin().z) * invDir.z;
		T t6 = (aAABB.GetMax().z - aRay.GetOrigin().z) * invDir.z;

		T tMin = CommonUtilities::Max(CommonUtilities::Max(CommonUtilities::Min(t1, t2), CommonUtilities::Min(t3, t4)), CommonUtilities::Min(t5, t6));
		T tMax = CommonUtilities::Min(CommonUtilities::Min(CommonUtilities::Max(t1, t2), CommonUtilities::Max(t3, t4)), CommonUtilities::Max(t5, t6));

		if (tMax < T{} || tMin > tMax)
		{
			return false;
		}

		//special case - ray origin inside AABB
		if (tMin < T{})
		{
			aOutIntersectionPoint = aRay.GetOrigin() + aRay.GetDirection() * tMax;
		}
		else
		{
			aOutIntersectionPoint = aRay.GetOrigin() + aRay.GetDirection() * tMin;
		}

		return true;
	}

	template <class T>
	bool IntersectionSphereRay(const Sphere<T>& aSphere, const Ray<T>& aRay, Vector3<T>& aOutIntersectionPoint)
	{
		Vector3<T> rayToSphere {aSphere.GetCenter() - aRay.GetOrigin()};
		T t{ rayToSphere.Dot(aRay.GetDirection()) };

		bool inside{ aSphere.IsInside(aRay.GetOrigin()) };

		//special case - ray from outside of sphere going away from sphere
		if (!inside && t < T{})
		{
			return false;
		}

		Vector3<T> closestPoint{aRay.GetOrigin() + aRay.GetDirection() * t};

		if (aSphere.IsInside(closestPoint))
		{
			T sphereRadius = aSphere.GetRadius();
			Vector3<T> yV{aSphere.GetCenter() - closestPoint};
			T ySqr{ yV.LengthSqr() };
			T x = std::sqrt((sphereRadius * sphereRadius) - ySqr);
			//special case - ray origin inside sphere
			if (inside) x = -x;
			aOutIntersectionPoint = aRay.GetOrigin() + aRay.GetDirection() * (t - x);
			return true;
		}
		else
		{
			return false;
		}
	}
}
