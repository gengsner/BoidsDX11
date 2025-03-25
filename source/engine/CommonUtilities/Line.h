#pragma once
#include "Vector2.h"
namespace CommonUtilities
{
	template <class T>
	class Line
	{
	public:
		Line();

		Line(const Line <T>& aLine);

		Line(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1);

		void InitWith2Points(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1);

		void InitWithPointAndDirection(const Vector2<T>& aPoint, const Vector2<T>& aDirection);

		bool IsInside(const Vector2<T>& aPosition) const;

		const Vector2<T>& GetDirection() const;

		const Vector2<T>& GetNormal() const;

	private:
		Vector2<T> myPoint;
		Vector2<T> myDirection;
		Vector2<T> myNormal;
	};

	template<class T>
	inline Line<T>::Line()
		: myPoint{},
		myDirection{},
		myNormal{}
	{}

	template<class T>
	inline Line<T>::Line(const Line<T>& aLine)
		: myPoint{ aLine.myPoint0 },
		myDirection{ aLine.myDirection },
		myNormal{ aLine.myNormal }
	{}

	template<class T>
	inline Line<T>::Line(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1)
		: Line<T>()
	{
		InitWith2Points(aPoint0, aPoint1);
	}

	template<class T>
	inline void Line<T>::InitWith2Points(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1)
	{
		myPoint = aPoint0;
		myDirection = (aPoint1 - aPoint0).GetNormalized();
		myNormal = { -myDirection.y, myDirection.x };
	}

	template<class T>
	inline void Line<T>::InitWithPointAndDirection(const Vector2<T>& aPoint, const Vector2<T>& aDirection)
	{
		myPoint = aPoint;
		myDirection = aDirection.GetNormalized();
		myNormal = { -myDirection.y, myDirection.x };
	}

	template<class T>
	inline bool Line<T>::IsInside(const Vector2<T>& aPosition) const
	{
		return myNormal.Dot({ myPoint - aPosition }) >= T{};
	}

	template<class T>
	inline const Vector2<T>& Line<T>::GetDirection() const
	{

		return myDirection;
	}

	template<class T>
	inline const Vector2<T>& Line<T>::GetNormal() const
	{
		return myNormal;
	}
}