#pragma once
#include "Vector3.h"

namespace CommonUtilities
{
	template <class T>
	class Plane
	{
	public:
		Plane();

		Plane(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2);

		Plane(const Vector3<T>& aPoint, const Vector3<T>& aNormal);

		void InitWith3Points(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2);

		void InitWithPointAndNormal(const Vector3<T>& aPoint, const Vector3<T>& aNormal);

		bool IsInside(const Vector3<T>& aPosition) const;

		const Vector3<T>& GetNormal() const;

		const Vector3<T>& GetPoint() const;

	private:
		Vector3<T> myPoint;
		Vector3<T> myNormal;
	};

	template<class T>
	inline Plane<T>::Plane()
		: myPoint{},
		myNormal{}
	{}

	template<class T>
	inline Plane<T>::Plane(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2)
		: Plane<T>()
	{
		InitWith3Points(aPoint0, aPoint1, aPoint2);
	}

	template<class T>
	inline Plane<T>::Plane(const Vector3<T>& aPoint, const Vector3<T>& aNormal)
		: myPoint{ aPoint },
		myNormal{ aNormal }
	{}

	template<class T>
	inline void Plane<T>::InitWith3Points(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2)
	{
		myPoint = aPoint0;
		auto dir1{ aPoint1 - myPoint };
		auto dir2{ aPoint2 - myPoint };
		myNormal = dir1.Cross(dir2).GetNormalized();
	}

	template<class T>
	inline void Plane<T>::InitWithPointAndNormal(const Vector3<T>& aPoint, const Vector3<T>& aNormal)
	{
		myPoint = aPoint;
		myNormal = aNormal;
	}

	template<class T>
	inline bool Plane<T>::IsInside(const Vector3<T>& aPosition) const
	{
		return myNormal.Dot({ myPoint - aPosition }) >= T{};
	}

	template<class T>
	inline const Vector3<T>& Plane<T>::GetNormal() const
	{
		return myNormal;
	}

	template<class T>
	inline const Vector3<T>& Plane<T>::GetPoint() const
	{
		return myPoint;
	}
}