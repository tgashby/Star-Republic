#ifndef _SVECTOR3_H_INCLUDED_
#define _SVECTOR3_H_INCLUDED_

#include <math.h>

class SVector3
{

public:

	float X, Y, Z;

	SVector3()
		: X(0), Y(0), Z(0)
	{}

	SVector3(float in)
		: X(in), Y(in), Z(in)
	{}

	SVector3(float in_x, float in_y, float in_z)
		: X(in_x), Y(in_y), Z(in_z)
	{}

	SVector3 crossProduct(SVector3 const & v) const
	{
		return SVector3(Y*v.Z - v.Y*Z, v.X*Z - X*v.Z, X*v.Y - v.X*Y);
	}

	float dotProduct(SVector3 const & v) const
	{
		return X*v.X + Y*v.Y + Z*v.Z;
	}

	float length() const
	{
		return sqrtf(X*X + Y*Y + Z*Z);
	}

	SVector3 operator + (SVector3 const & v) const
	{
		return SVector3(X+v.X, Y+v.Y, Z+v.Z);
	}

	SVector3 & operator += (SVector3 const & v)
	{
		X += v.X;
		Y += v.Y;
		Z += v.Z;

		return * this;
	}

	SVector3 operator - (SVector3 const & v) const
	{
		return SVector3(X-v.X, Y-v.Y, Z-v.Z);
	}

	SVector3 & operator -= (SVector3 const & v)
	{
		X -= v.X;
		Y -= v.Y;
		Z -= v.Z;

		return * this;
	}

	SVector3 operator * (SVector3 const & v) const
	{
		return SVector3(X*v.X, Y*v.Y, Z*v.Z);
	}

	SVector3 & operator *= (SVector3 const & v)
	{
		X *= v.X;
		Y *= v.Y;
		Z *= v.Z;

		return * this;
	}

	SVector3 operator / (SVector3 const & v) const
	{
		return SVector3(X/v.X, Y/v.Y, Z/v.Z);
	}

	SVector3 & operator /= (SVector3 const & v)
	{
		X /= v.X;
		Y /= v.Y;
		Z /= v.Z;

		return * this;
	}

	SVector3 operator * (float const s) const
	{
		return SVector3(X*s, Y*s, Z*s);
	}

	SVector3 & operator *= (float const s)
	{
		X *= s;
		Y *= s;
		Z *= s;

		return * this;
	}

	SVector3 operator / (float const s) const
	{
		return SVector3(X/s, Y/s, Z/s);
	}

	SVector3 & operator /= (float const s)
	{
		X /= s;
		Y /= s;
		Z /= s;

		return * this;
	}

};

#endif
