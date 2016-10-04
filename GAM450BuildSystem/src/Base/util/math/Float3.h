/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Float3.h 
 * Author: Jorge Antonio Robles
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#include "Base/util/math/Float2.h"

class float3
{
private:

	template<int a, int b, int c>
	class swizzle
	{
	public:

		operator float3(void)
		{
			return float3(v[a], v[b], v[c]);
		}

		operator float2(void)
		{
			return float2(v[a], v[b]);
		}

		float3& operator=(const float3& rhs)
		{
			ErrorIf((a == b || a == c || b == c), "Cannot use this swizzle for that operation");

			v[a] = rhs.x;
			v[b] = rhs.y;
			v[c] = rhs.z;

			return *(float3*)(this);
		}

		float3 operator-(void)
		{
			return float3(-v[a], -v[b], -v[c]);
		}

		float3 operator+(const float3& rhs) const
		{
			return float3(v[a] + rhs.x, v[b] + rhs.y, v[c] + rhs.z);
		}

		float3 operator-(const float3& rhs) const
		{
			return float3(v[a] - rhs.x, v[b] - rhs.y, v[c] - rhs.z);
		}

		float3 operator*(const float rhs) const
		{
			return float3(v[a] * rhs, v[b] * rhs, v[c] * rhs);
		}

		float3 operator/(const float rhs) const
		{
			return float3(v[a] / rhs, v[b] / rhs, v[c] / rhs);
		}

		float3& operator+=(const float3& rhs)
		{
			ErrorIf((a == b || a == c || b == c), "Cannot use this swizzle for that operation");

			v[a] += rhs.x;
			v[b] += rhs.y;
			v[c] += rhs.z;

			return *(float3*)(this);
		}

		float3& operator-=(const float3& rhs)
		{
			ErrorIf((a == b || a == c || b == c), "Cannot use this swizzle for that operation");

			v[a] -= rhs.x;
			v[b] -= rhs.y;
			v[c] -= rhs.z;

			return *(float3*)(this);
		}

		float3& operator*=(const float rhs)
		{
			ErrorIf((a == b || a == c || b == c), "Cannot use this swizzle for that operation");

			v[a] *= rhs;
			v[b] *= rhs;
			v[c] *= rhs;

			return *(float3*)(this);
		}

		float3& operator/=(const float rhs)
		{
			ErrorIf((a == b || a == c || b == c), "Cannot use this swizzle for that operation");

			v[a] /= rhs;
			v[b] /= rhs;
			v[c] /= rhs;

			return *(float3*)(this);
		}


	private:
		float v[3];

	};

public:


	//This is done so that the vector
	//can be accessed in multiple ways.
	union
	{
		struct
		{
			float x, y, z;
		};

		swizzle<0, 0, -1> xx;
		swizzle<0, 1, -1> xy;
		swizzle<0, 2, -1> xz;
		swizzle<1, 0, -1> yx;
		swizzle<1, 2, -1> yz;
		swizzle<1, 1, -1> yy;
		swizzle<2, 0, -1> zx;
		swizzle<2, 1, -1> zy;
		swizzle<2, 2, -1> zz;

		swizzle<0, 0, 0> xxx;
		swizzle<0, 0, 1> xxy;
		swizzle<0, 0, 2> xxz;
		swizzle<0, 1, 0> xyx;
		swizzle<0, 1, 1> xyy;
		swizzle<0, 1, 2> xyz;
		swizzle<0, 2, 0> xzx;
		swizzle<0, 2, 1> xzy;
		swizzle<0, 2, 2> xzz;
		swizzle<1, 0, 0> yxx;
		swizzle<1, 0, 1> yxy;
		swizzle<1, 0, 2> yxz;
		swizzle<1, 1, 0> yyx;
		swizzle<1, 1, 1> yyy;
		swizzle<1, 1, 2> yyz;
		swizzle<1, 2, 0> yzx;
		swizzle<1, 2, 1> yzy;
		swizzle<1, 2, 2> yzz;
		swizzle<2, 0, 0> zxx;
		swizzle<2, 0, 1> zxy;
		swizzle<2, 0, 2> zxz;
		swizzle<2, 1, 0> zyx;
		swizzle<2, 1, 1> zyy;
		swizzle<2, 1, 2> zyz;
		swizzle<2, 2, 0> zzx;
		swizzle<2, 2, 1> zzy;
		swizzle<2, 2, 2> zzz;

		float v[3];
	};

	//Default constructor
	float3(void);

	//Copy constructor
	float3(const float3& rhs);
	float3(const float2& rhs, float zz);
	float3(float xx, const float2& rhs);

	//Non-default constructor
	float3(float xx, float yy, float zz);

	//Operator overloads
	float3& operator=(const float3& rhs); //Assignment
	float3 operator-(void) const; //Unary negation operator

	float3 operator+(const float3& rhs) const;
	float3 operator-(const float3& rhs) const;
	float3 operator*(const float rhs) const;
	float3 operator/(const float rhs) const;

	float3& operator+=(const float3& rhs);
	float3& operator-=(const float3& rhs);
	float3& operator*=(const float rhs);
	float3& operator/=(const float rhs);

	bool operator==(const float3& rhs) const;
	bool operator!=(const float3& rhs) const;

	//Vector operations
	float Dot(const float3& rhs) const;
	float3 Cross(const float3& rhs) const;
	float Length(void) const;
	float LengthSq(void) const;
	float3 Normalize(void);
	void Zero(void);
	void Print(void) const;

	static const float3 cZero;
	static const float3 cXAxis;
	static const float3 cYAxis;
	static const float3 cZAxis;
};
