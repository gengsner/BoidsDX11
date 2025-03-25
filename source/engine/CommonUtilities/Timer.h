#pragma once

namespace CommonUtilities
{
	class Timer
	{
	public:
		Timer();
		Timer(const Timer& aTimer) = delete;
		Timer& operator=(const Timer& aTimer) = delete;

		void Update();

		float GetDeltaTime() const;
		double GetTotalTime() const;

	private:
		float myDeltaTime;
		double myTotalTime;
		double myCurrTime;
		double myPrevTime;
	};
}
