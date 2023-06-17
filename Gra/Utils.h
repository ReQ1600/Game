#pragma once
#include <vector>
#include <iostream>
#include <SFML/System.hpp>

constexpr double PI = 3.14159265359;

typedef sf::Vector2<__int8> vec2c;
typedef sf::Vector2<double> Vertex;
typedef sf::Vector2<double> vec2d;
typedef std::vector<Vertex> Vertices;

namespace Utils
{
	template<class T>
	void Log(T t)
	{
		std::cout << t << std::endl;
	};

	
	template<class T>
	void Log(std::vector<T> t)
	{
		std::cout << "Vector of " << typeid(T).name() << '\n';
		for (int i = 0; i < t.size(); ++i)
		{
			std::cout << i << "  ";
			Log<T>(t[i]);
		}

	}

	template<class T>
	bool ErrorLog(T t)
	{
		std::cout << "Error: \n" << t << std::endl;
		return false;
	}

	template<class T, class N>
	void Hit(T& thing, const N& projectile)
	{
		thing.setHealth(thing.getHealth() - projectile.getDamage());
		if (thing.getHealth() <= 0) thing.Die();
	}

	template<typename T>
	T Deg2Rad(const T& deg)
	{
		if (!std::is_arithmetic(deg))
		{
			Log("type is not a number");
			return 0;
		}
		return deg * PI / 180;
	}

	template<typename T>
	void RotatePoint(Vertex& point, const Vertex& rotation_center, const T& deg)
	{
		const double angle = Deg2Rad(deg);
		Vertex translatedPoint(point.x - rotation_center.x, point.y - rotation_center.y); //translation to 0,0
		point = { translatedPoint.x * cos(-angle) - translatedPoint.y * sin(-angle) + rotation_center.x,
				  translatedPoint.x * sin(-angle) + translatedPoint.y * cos(-angle) + rotation_center.y };
	}

	inline void OffsetVertices(Vertices& vertices, const vec2d& offset)
	{
		for (auto& [x,y] : vertices)
		{
			x += offset.x;
			y += offset.y;
		}
	}

	inline void FlipVerticesX(Vertices& vertices, const double& center)
	{
		for (Vertex& v : vertices)
		{
			v.x -= center;
			v.x *= -1;
			v.x += center;
		}
	}

	inline double CalculateMagnitude(const vec2d &vector)
	{
		return std::sqrt(vector.x * vector.x + vector.y * vector.y);
	}

	inline vec2d CalculateUnitVector(const Vertex& pointA, const Vertex& pointB)
	{
		Vertex tmp = pointB - pointA;
		double mag = CalculateMagnitude(tmp);
		return tmp / mag;
	}

	inline double Distance2Points(const vec2d& p1, const vec2d& p2)
	{
		double a = (p2.x - p1.x) * (p2.x - p1.x);
		double b = (p2.y - p1.y) * (p2.y - p1.y);
		return std::sqrt(a + b);
	}
	inline double Distance2Points(const sf::Vector2f& p1, const vec2d& p2)
	{
		double a = (p2.x - p1.x) * (p2.x - p1.x);
		double b = (p2.y - p1.y) * (p2.y - p1.y);
		return std::sqrt(a + b);
	}
	inline double Distance2Points(const sf::Vector2f& p1, const sf::Vector2f& p2)
	{
		double a = (p2.x - p1.x) * (p2.x - p1.x);
		double b = (p2.y - p1.y) * (p2.y - p1.y);
		return std::sqrt(a + b);
	}//that many functions bc its annoying to constantly cast
	
}