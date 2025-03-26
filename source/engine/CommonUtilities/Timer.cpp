#include "Timer.h"
#include <chrono>

namespace CommonUtilities
{
	Timer::Timer() :
		myDeltaTime{},
		myTotalTime{},
		myPrevTime{},
		myCurrTime{}
	{
		myPrevTime = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	}

	void Timer::Update()
	{
		myCurrTime = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
		myDeltaTime = static_cast<float>(myCurrTime - myPrevTime);
		myTotalTime += (myCurrTime - myPrevTime);
		myPrevTime = myCurrTime;
	}

	float Timer::GetDeltaTime() const
	{
		return myDeltaTime;
	}

	double Timer::GetTotalTime() const
	{
		return myTotalTime;
	}
}
