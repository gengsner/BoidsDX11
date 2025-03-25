#pragma once
#include <cassert>
#include <cmath>
#include <utility>

namespace CommonUtilities
{
	template <class T>
	class Vector3
	{
	public:
		T x;
		T y;
		T z;

		Vector3<T>();

		Vector3<T>(const T& aX, const T& aY, const T& aZ);

		Vector3<T>(const std::initializer_list<T>& aList);

		Vector3<T>(const Vector3<T>& aVector) = default;

		Vector3<T>& operator=(const Vector3<T>& aVector3) = default;

		~Vector3<T>() = default;

		T LengthSqr() const;

		T Length() const;

		Vector3<T> GetNormalized() const;

		void Normalize();

		T Dot(const Vector3<T>& aVector) const;

		Vector3<T> Cross(const Vector3<T>& aVector) const;

	};

	template <class T> Vector3<T> operator+(const Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		return{ aVector0.x + aVector1.x, aVector0.y + aVector1.y, aVector0.z + aVector1.z };
	}

	template <class T> Vector3<T> operator-(const Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		return{ aVector0.x - aVector1.x, aVector0.y - aVector1.y, aVector0.z - aVector1.z };
	}

	template <class T> Vector3<T> operator*(const Vector3<T>& aVector, const T& aScalar)
	{
		return{ aVector.x * aScalar, aVector.y * aScalar, aVector.z * aScalar };
	}

	template <class T> Vector3<T> operator*(const T& aScalar, const Vector3<T>& aVector)
	{
		return aVector * aScalar;
	}

	template <class T> Vector3<T> operator/(const Vector3<T>& aVector, const T& aScalar)
	{
		return{ aVector.x / aScalar, aVector.y / aScalar, aVector.z / aScalar};
	}

	template <class T> void operator+=(Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		aVector0 = aVector0 + aVector1;
	}

	template <class T> void operator-=(Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		aVector0 = aVector0 - aVector1;
	}

	template <class T> void operator*=(Vector3<T>& aVector, const T& aScalar)
	{
		aVector = aVector * aScalar;
	}

	template <class T> void operator/=(Vector3<T>& aVector, const T& aScalar)
	{
		aVector = aVector / aScalar;
	}

	template <class T> bool operator!=(const Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		return aVector0.x != aVector1.x
			|| aVector0.y != aVector1.y
			|| aVector0.z != aVector1.z;
	}

	template<class T>
	inline Vector3<T>::Vector3() : x{}, y{}, z{}
	{}

	template<class T>
	inline Vector3<T>::Vector3(const T & aX, const T & aY, const T & aZ) : x{ aX }, y{ aY }, z{ aZ }
	{
	}

	template<class T>
	inline Vector3<T>::Vector3(const std::initializer_list<T>& aList):
		Vector3<T>()
	{
#ifdef _DEBUG
		int listSize{ (int)aList.size() };
		assert(listSize == 3);
#endif 
		x = *(aList.begin() + 0);
		y = *(aList.begin() + 1);
		z = *(aList.begin() + 2);
	}

	template<class T>
	inline T Vector3<T>::LengthSqr() const
	{
		return x * x + y * y + z * z;
	}

	template<class T>
	inline T Vector3<T>::Length() const
	{
		return std::sqrt(LengthSqr());
	}

	template<class T>
	inline Vector3<T> Vector3<T>::GetNormalized() const
	{
		T length{ Length() };
		if (length == T{})
		{
			return { T{}, T{}, T{} };
		}
		return{ x / length, y / length, z / length };
	}

	template<class T>
	inline void Vector3<T>::Normalize()
	{
		T length{ Length() };
		if (length == T{})
		{
			return;
		}
		x /= length;
		y /= length;
		z /= length;
	}

	template<class T>
	inline T Vector3<T>::Dot(const Vector3<T>& aVector) const
	{
		return x * aVector.x + y * aVector.y + z * aVector.z;
	}
	template<class T>
	inline Vector3<T> Vector3<T>::Cross(const Vector3<T>& aVector) const
	{
		return{ y * aVector.z - z * aVector.y, z * aVector.x - x * aVector.z, x * aVector.y - y * aVector.x };
	}
}