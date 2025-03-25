#pragma once
#include "Vector3.h"

namespace CommonUtilities
{
	template <class T>
	class Ray
	{
	public:
		// Default constructor: there is no ray, the origin and direction are the 
		// zero vector.
		Ray();

		// Copy constructor.
		Ray(const Ray<T>& aRay);

		// Constructor that takes two points that define the ray, the direction is 
		// aDirectionm and the origin is aOrigin.
		Ray(const Vector3<T>& aOrigin, const Vector3<T>& aDirection);

		// Init the ray with two points, the same as the constructor above.
		void InitWith2Points(const Vector3<T>& aOrigin, const Vector3<T>& aPoint);

		// Init the ray with an origin and a direction.
		void InitWithOriginAndDirection(const Vector3<T>& aOrigin, const Vector3<T>& aDirection);

		const Vector3<T>& GetOrigin() const;

		const Vector3<T>& GetDirection() const;

	private:
		Vector3<T> myOrigin;
		Vector3<T> myDirection;
	};
	template<class T>
	inline Ray<T>::Ray()
		: myOrigin{},
		myDirection{}
	{}

	template<class T>
	inline Ray<T>::Ray(const Ray<T>& aRay)
		: myOrigin{ aRay.myOrigin },
		myDirection{ aRay.myDirection }
	{}

	template<class T>
	inline Ray<T>::Ray(const Vector3<T>& aOrigin, const Vector3<T>& aDirection)
		: myOrigin{ aOrigin },
		myDirection{ aDirection.GetNormalized() }
	{}

	template<class T>
	inline void Ray<T>::InitWith2Points(const Vector3<T>& aOrigin, const Vector3<T>& aPoint)
	{
		myOrigin = aOrigin;
		myDirection = aPoint - aOrigin;
		myDirection.Normalize();
	}

	template<class T>
	inline void Ray<T>::InitWithOriginAndDirection(const Vector3<T>& aOrigin, const Vector3<T>& aDirection)
	{
		myOrigin = aOrigin;
		myDirection = aDirection;
		myDirection.Normalize();
	}

	template<class T>
	const Vector3<T>& Ray<T>::GetOrigin() const
	{
		return myOrigin;
	}

	template<class T>
	const Vector3<T>& Ray<T>::GetDirection() const
	{
		return myDirection;
	}
}