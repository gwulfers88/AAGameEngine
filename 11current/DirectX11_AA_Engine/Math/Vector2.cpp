/*
AA Engine
Copy right: Zhu,Wei Visin
Date: 17_12_2015
*/
#include "Vector2.h"

#define PI 3.141592654

namespace AAEngine{

	Vector2::Vector2()
	{
		x = 0.0f;
		y = 0.0f;
	}

	Vector2::Vector2(float ax, float ay)
	{
		x = ax;
		y = ay;
	}
	//===========================================
	Vector2 Vector2::operator-() const
	{
		Vector2 result;
		result.x = -x;
		result.y = -y;
		return result;
	}

	Vector2 AAEngine::Vector2::operator+(const Vector2& other) const
	{
		Vector2 result;
		result.x = x + other.x;
		result.y = y + other.y;
		return result;
	}

	Vector2 AAEngine::Vector2::operator-(const Vector2& other) const
	{
		Vector2 result;
		result.x = x - other.x;
		result.y = y - other.y;
		return result;
	}

	Vector2 Vector2::operator=(const Vector2& copy)
	{
		x = copy.x;
		y = copy.y;
		return copy;
	}

	Vector2 Vector2::operator*(float c) const
	{
		Vector2 result;
		result.x = x*c;
		result.y = y*c;
		return result;
	}

	Vector2 operator*(float c, const Vector2& v)
	{
		return v * c;
	}

	Vector2 Vector2::operator/(float c) const
	{
		Vector2 result;
		result.x = x / c;
		result.y = y / c;
		return result;
	}
	float Vector2::squareMagnitude()
	{
		return Dot(Vector2(x, y), Vector2(x, y));
	}

	float Vector2::magnitude()
	{
		return sqrt(squareMagnitude());
	}
	Vector2 Vector2::normalze()
	{
		return Vector2(x / magnitude(), y / magnitude());
	}
	//================================================================================
	float Dot(const Vector2& a, const Vector2& b)
	{
		return (a.x * b.x + a.y * b.y);
	}
	float AngleBetween(Vector2& a, Vector2& b)
	{
		float dot = Dot(a, b);
		return  acos(dot / (a.magnitude() * b.magnitude())) * 180.0f / PI;
	}
	float distance(const Vector2& a, const Vector2& b)
	{
		return (a - b).magnitude();
	}
	bool IsPerpendicular(const Vector2& a, const Vector2& b)
	{
		return (a.x*a.y + b.x*b.y) == 0;
	}
	bool IsParalle(const Vector2& a, const Vector2& b)
	{
		return (a.x*b.y - b.x*a.y) == 0;
	}
	Vector2 ProjectionA_On_B(Vector2& a, Vector2& b)
	{
		//return a.magnitude() * cos((AngleBetween(a, b))*PI/180.0f) *b.normalze();
		return b * (Dot(a, b) / Dot(b, b));
	}
}