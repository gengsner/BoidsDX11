#pragma once
constexpr double PI{ 3.141592653589793 };

namespace CommonUtilities
{
	template <class T>
	class Vector2
	{
	public:
		T x;
		T y;

		Vector2<T>();

		Vector2<T>(const T& aX, const T& aY);

		Vector2<T>(const Vector2<T>& aVector) = default;

		Vector2<T>& operator=(const Vector2<T>& aVector2) = default;

		~Vector2<T>() = default;

		T LengthSqr() const;

		T Length() const;

		Vector2<T> GetNormalized() const;

		void Normalize();

		T Dot(const Vector2<T>& aVector) const;
	};

	template <class T> Vector2<T> operator+(const Vector2<T>& aVector0, const Vector2<T>& aVector1) 
	{
		return{ aVector0.x + aVector1.x, aVector0.y + aVector1.y };
	}

	template <class T> Vector2<T> operator-(const Vector2<T>& aVector0, const Vector2<T>& aVector1) 
	{
		return{ aVector0.x - aVector1.x, aVector0.y - aVector1.y };
	}

	template <class T> Vector2<T> operator*(const Vector2<T>& aVector, const T& aScalar) 
	{
		return{ aVector.x * aScalar, aVector.y * aScalar };
	}

	template <class T> Vector2<T> operator*(const T& aScalar, const Vector2<T>& aVector) 
	{
		return aVector * aScalar;
	}

	template <class T> Vector2<T> operator/(const Vector2<T>& aVector, const T& aScalar) 
	{
		return{ aVector.x / aScalar, aVector.y / aScalar };
	}

	template <class T> void operator+=(Vector2<T>& aVector0, const Vector2<T>& aVector1) 
	{
		aVector0 = aVector0 + aVector1;
	}

	template <class T> void operator-=(Vector2<T>& aVector0, const Vector2<T>& aVector1) 
	{
		aVector0 = aVector0 - aVector1;
	}

	template <class T> void operator*=(Vector2<T>& aVector, const T& aScalar) 
	{
		aVector = aVector * aScalar;
	}

	template <class T> void operator/=(Vector2<T>& aVector, const T& aScalar) 
	{
		aVector = aVector / aScalar;
	}
	
	template<class T>
	inline Vector2<T>::Vector2() : x{}, y{}
	{}

	template<class T>
	inline Vector2<T>::Vector2(const T& aX, const T& aY) : x{aX}, y{aY}
	{}

	template<class T>
	inline T Vector2<T>::LengthSqr() const
	{
		return x * x + y * y;
	}

	template<class T>
	inline T Vector2<T>::Length() const
	{
		return std::sqrt(LengthSqr());
	}

	template<class T>
	inline Vector2<T> Vector2<T>::GetNormalized() const
	{
		T length{ Length() };
		if (length == T{})
		{
			return { T{}, T{} };
		}
		return{ x / length, y / length };
	}

	template<class T>
	inline void Vector2<T>::Normalize()
	{
		T length{ Length() };
		if (length == T{})
		{
			return;
		}
		x /= length;
		y /= length;
	}

	template<class T>
	inline T Vector2<T>::Dot(const Vector2<T>& aVector) const
	{
		return x * aVector.x + y * aVector.y;
	}
}
