/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Float2.h 
 * Author: Jorge Antonio Robles
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once

#include "Base/util/debug/DebugAssist.h"

class float2
{
private:
	
	template<int a, int b>
	class swizzle
	{
		public:
			
			operator float2(void)
			{
				return float2(v[a], v[b]);
			}

			float2& operator=(const float2& rhs)
			{
				ErrorIf((a == b), "Cannot use this swizzle for that operation");

				v[a] = rhs.x;
				v[b] = rhs.y;

				return *(float2*)(this);
			}

			float2 operator-(void)
			{
				return float2(-v[a], -v[b]);
			}

			float2 operator+(const float2& rhs) const
			{
				return float2(v[a] + rhs.x, v[b] + rhs.y);
			}

			float2 operator-(const float2& rhs) const
			{
				return float2(v[a] - rhs.x, v[b] - rhs.y);
			}

			float2 operator*(const float rhs) const
			{
				return float2(v[a] * rhs, v[b] * rhs);
			}

			float2 operator/(const float rhs) const
			{
				return float2(v[a] / rhs, v[b] / rhs);
			}

			float2& operator+=(const float2& rhs)
			{
				ErrorIf((a == b), "Cannot use this swizzle for that operation");

				v[a] += rhs.x;
				v[b] += rhs.y;

				return *(float2*)(this);
			}

			float2& operator-=(const float2& rhs)
			{
				ErrorIf((a == b), "Cannot use this swizzle for that operation");

				v[a] -= rhs.x;
				v[b] -= rhs.y;

				return *(float2*)(this);
			}

			float2& operator*=(const float rhs)
			{
				ErrorIf((a == b), "Cannot use this swizzle for that operation");

				v[a] *= rhs;
				v[b] *= rhs;

				return *(float2*)(this);
			}

			float2& operator/=(const float rhs)
			{
				ErrorIf((a == b), "Cannot use this swizzle for that operation");

				v[a] /= rhs;
				v[b] /= rhs;

				return *(float2*)(this);
			}


		private:
			float v[2];

	};

	public:


		//This is done so that the vector
		//can be accessed in multiple ways.
		//float2 vec;
		//vec.x is valid
		//vec.v[0] is also valid
		union
		{ 
			struct
			{
				float x, y;
			};

			swizzle<0, 0> xx;
			swizzle<0, 1> xy;
			swizzle<1, 0> yx;
			swizzle<1, 1> yy;

			float v[2];
		};

		//Default constructor
		float2(void);

		//Copy constructor
		float2(const float2& rhs);

		//Non-default constructor
		float2(float xx, float yy);

		//Operator overloads
		float2& operator=(const float2& rhs); //Assignment
		float2 operator-(void) const; //Unary negation operator

		float2 operator+(const float2& rhs) const;
		float2 operator-(const float2& rhs) const;
		float2 operator*(const float rhs) const;
		float2 operator/(const float rhs) const;

		float2& operator+=(const float2& rhs);
		float2& operator-=(const float2& rhs);
		float2& operator*=(const float rhs);
		float2& operator/=(const float rhs);

		bool operator==(const float2& rhs) const;
		bool operator!=(const float2& rhs) const;
		
		//Vector operations
		float Dot(const float2& rhs) const;
		float Length(void) const;
		float LengthSq(void) const;
		void Normalize(void);
		void Zero(void);
		void Print(void) const;
};
