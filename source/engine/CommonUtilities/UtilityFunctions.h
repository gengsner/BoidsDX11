#pragma once
#include <cassert>

namespace CommonUtilities
{
	template<typename T>
	const T& Min(const T& aValX, const T& aValY)
	{
		return aValX < aValY ? aValX : aValY;
	}

	template<typename T>
	const T& Max(const T& aValX, const T& aValY)
	{
		return aValX < aValY ? aValY : aValX;
	}

	template<typename T>
	T Abs(const T& aVal)
	{
		return aVal < 0 ? -aVal : aVal;
	}

	template<typename T>
	const T& Clamp(const T& aVal, const T& aLow, const T& aHigh)
	{
		assert(aLow <= aHigh && "Min larger than Max!");
		return aVal < aLow ? aLow : aHigh < aVal ? aHigh : aVal;
	}

	template<typename T>
	T Lerp(const T& aValX, const T& aValY, const float& aPercentage)
	{
		return aValX + aPercentage * (aValY - aValX);
	}

	template<typename T>
	void Swap(T& aValX, T& aValY)
	{
		T temp{ aValX };
		aValX = aValY;
		aValY = temp;
	}
}

