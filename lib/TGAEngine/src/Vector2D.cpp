#include "Vector2D.h"
#include <math.h>

namespace TGA
{
	Vector2D::Vector2D(float xComp, float yComp)
		: x(xComp), y(yComp) {}


	Vector2D::~Vector2D(void) {}

	Vector2D Vector2D::operator+(Vector2D vec2)
	{
		return Vector2D(this->x + vec2.x, this->y + vec2.y);
	}

	Vector2D Vector2D::operator-(Vector2D vec2)
	{
		return Vector2D(this->x - vec2.x, this->y - vec2.y);
	}

	Vector2D Vector2D::scalarProduct(int mult)
	{
		return Vector2D(this->x * mult, this->y * mult);
	}

	Vector2D Vector2D::normalize()
	{		
		return Vector2D(this->x / length(), this->y / length());
	}

	float Vector2D::dotProduct(Vector2D vec2)
	{
		return this->x * vec2.x + this->y * vec2.y;
	}

	float Vector2D::length()
	{
		return sqrt(this->x * this->x + this->y * this->y);
	}

	float Vector2D::distanceFrom(Vector2D vec2)
	{
		return (*this - vec2).length();
	}

	float Vector2D::getX()
	{
		return x;
	}

	float Vector2D::getY()
	{
		return y;
	}
}

