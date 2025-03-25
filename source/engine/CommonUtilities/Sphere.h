#pragma once
#include "Vector3.h"

namespace CommonUtilities
{
	template <class T>
	class Sphere
	{
	public:
		Sphere();

		Sphere(const Sphere<T>& aSphere);

		Sphere(const Vector3<T>& aCenter, T aRadius);

		void InitWithCenterAndRadius(const Vector3<T>& aCenter, T aRadius);

		bool IsInside(const Vector3<T>& aPosition) const;

		const Vector3<T>& GetCenter() const;

		const T& GetRadius() const;

	private:
		Vector3<T> myCenter;
		T myRadius;
	};

	template<class T>
	inline Sphere<T>::Sphere()
		: myCenter{}, 
		myRadius{}
	{}

	template<class T>
	inline Sphere<T>::Sphere(const Sphere<T>& aSphere)
		: myCenter{ aSphere.myCenter },
		myRadius{ aSphere.myRadius }
	{}

	template<class T>
	inline Sphere<T>::Sphere(const Vector3<T>& aCenter, T aRadius)
		: myCenter{ aCenter },
		myRadius{ aRadius }
	{}

	template<class T>
	inline void Sphere<T>::InitWithCenterAndRadius(const Vector3<T>& aCenter, T aRadius)
	{
		myCenter = aCenter;
		myRadius = aRadius;
	}

	template<class T>
	inline bool Sphere<T>::IsInside(const Vector3<T>& aPosition) const
	{
		Vector3<T> dist = myCenter - aPosition;
		return dist.LengthSqr() <= myRadius * myRadius;
	}
	
	template<class T>
	const Vector3<T>& Sphere<T>::GetCenter() const
	{
		return myCenter;
	}

	template<class T>
	const T& Sphere<T>::GetRadius() const
	{
		return myRadius;
	}
}