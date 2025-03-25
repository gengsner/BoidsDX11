#pragma once
#include <initializer_list>

namespace CommonUtilities
{
	template <class T>
	class Vector4
	{
	public:
		T x;
		T y;
		T z;
		T w;

		Vector4<T>();

		Vector4<T>(const T& aX, const T& aY, const T& aZ, const T& aW);

		Vector4<T>(const std::initializer_list<T>& aList);

		Vector4<T>(const Vector4<T>& aVector) = default;

		Vector4<T>& operator=(const Vector4<T>& aVector4) = default;

		~Vector4<T>() = default;

		T LengthSqr() const;

		T Length() const;

		Vector4<T> GetNormalized() const;

		void Normalize();

		T Dot(const Vector4<T>& aVector) const;
	};

	template <class T> Vector4<T> operator+(const Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{
		return{ aVector0.x + aVector1.x, aVector0.y + aVector1.y,  aVector0.z + aVector1.z, aVector0.w + aVector1.w };
	}

	template <class T> Vector4<T> operator-(const Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{
		return{ aVector0.x - aVector1.x, aVector0.y - aVector1.y,  aVector0.z - aVector1.z, aVector0.w - aVector1.w };
	}

	template <class T> Vector4<T> operator*(const Vector4<T>& aVector, const T& aScalar)
	{
		return{ aVector.x * aScalar, aVector.y * aScalar, aVector.z * aScalar, aVector.w * aScalar };
	}

	template <class T> Vector4<T> operator*(const T& aScalar, const Vector4<T>& aVector)
	{
		return aVector * aScalar;
	}

	template <class T> Vector4<T> operator/(const Vector4<T>& aVector, const T& aScalar)
	{
		return{ aVector.x / aScalar, aVector.y / aScalar, aVector.z / aScalar, aVector.w /  aScalar};
	}

	template <class T> void operator+=(Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{
		aVector0 = aVector0 + aVector1;
	}

	template <class T> void operator-=(Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{
		aVector0 = aVector0 - aVector1;
	}

	template <class T> void operator*=(Vector4<T>& aVector, const T& aScalar)
	{
		aVector = aVector * aScalar;
	}

	template <class T> void operator/=(Vector4<T>& aVector, const T& aScalar)
	{
		aVector = aVector / aScalar;
	}

	template<class T>
	inline Vector4<T>::Vector4() : x{}, y{}, z{}, w{}
	{}

	template<class T>
	inline Vector4<T>::Vector4(const T& aX, const T& aY, const T& aZ, const T& aW) : x{ aX }, y{ aY }, z{ aZ }, w{ aW }
	{
	}

	template<class T>
	inline Vector4<T>::Vector4(const std::initializer_list<T>& aList):
		Vector4<T>()
	{
#ifdef _DEBUG
		int listSize{ (int)aList.size() };
		assert(listSize == 4);
#endif // _DEBUG

		x = *(aList.begin() + 0);
		y = *(aList.begin() + 1);
		z = *(aList.begin() + 2);
		w = *(aList.begin() + 3);
	}

	template<class T>
	inline T Vector4<T>::LengthSqr() const
	{
		return x * x + y * y + z * z + w * w;
	}

	template<class T>
	inline T Vector4<T>::Length() const
	{
		return std::sqrt(LengthSqr());
	}

	template<class T>
	inline Vector4<T> Vector4<T>::GetNormalized() const
	{
		T length{ Length() };
		if (length == T{})
		{
			return { T{}, T{}, T{}, T{} };
		}
		return{ x / length, y / length, z / length, w / length };
	}

	template<class T>
	inline void Vector4<T>::Normalize()
	{
		T length{ Length() };
		if (length == T{})
		{
			return;
		}
		x /= length;
		y /= length;
		z /= length;
		w /= length;
	}

	template<class T>
	inline T Vector4<T>::Dot(const Vector4<T>& aVector) const
	{
		return x * aVector.x + y * aVector.y + z * aVector.z + w * aVector.w;
	}
}