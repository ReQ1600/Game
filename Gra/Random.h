#pragma once
#include <random>
namespace Utils
{
	class Random
	{
	public:
		Random(const Random&) = delete;
		~Random() {};
		static Random& Get() { return s_Instance; }

		template <typename T>
		static T RandomNumber(T min, T max) {
			std::random_device dev;
			std::mt19937 mt(dev());
			std::uniform_real_distribution<float> dist(min, max);
			return (T)dist(dev);
		};
	private:
		Random() {};
		static Random s_Instance;
	};
}
Utils::Random Utils::Random::s_Instance;
