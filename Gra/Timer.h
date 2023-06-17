#pragma once
#include <chrono>
//dunno if ill use it, if the green txt is still here then i forgot
namespace Utils
{
	class Timer
	{
	public:
		Timer() { Reset(); };
		~Timer() {};

		inline void Reset() { m_startTime = std::chrono::system_clock::now(); };

		template<typename T>
		inline T getElapsedTime() { return std::chrono::duration_cast<T>(std::chrono::system_clock::now() - m_startTime); };//returns elapsed time

	private:
		std::chrono::system_clock::time_point m_startTime;
	};
}