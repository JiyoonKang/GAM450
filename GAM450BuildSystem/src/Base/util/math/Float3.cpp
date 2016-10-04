/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Float3.cpp 
 * Author: Jorge Antonio Robles
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"
#include "float3.h"
#include <cmath>

static const float EPSILON = 0.0001f;

const float3 float3::cZero(0.0f, 0.0f, 0.0f);
const float3 float3::cXAxis(1.0f, 0.0f, 0.0f);
const float3 float3::cYAxis(0.0f, 1.0f, 0.0f);
const float3 float3::cZAxis(0.0f, 0.0f, 1.0f);

float3::float3(void) : x(0.0f), y(0.0f), z(0.0f)
{
}

float3::float3(const float3& rhs) : x(rhs.x), y(rhs.y), z(rhs.z)
{
}

float3::float3(const float2& rhs, float zz) : x(rhs.x), y(rhs.y), z(zz)
{
}

float3::float3(float xx, const float2& rhs) : x(xx), y(rhs.x), z(rhs.y)
{
}

float3::float3(float xx, float yy, float zz) : x(xx), y(yy), z(zz)
{
}

float3& float3::operator=(const float3& rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;

	return *this;
}

float3 float3::operator-(void) const
{
	float3 temp;

	temp.x = -x;
	temp.y = -y;
	temp.z = -z;

	return temp;
}

float3 float3::operator+(const float3& rhs) const
{
	float3 temp;

	temp.x = x + rhs.x;
	temp.y = y + rhs.y;
	temp.z = z + rhs.z;

	return temp;
}

float3 float3::operator-(const float3& rhs) const
{
	float3 temp;

	temp.x = x - rhs.x;
	temp.y = y - rhs.y;
	temp.z = z - rhs.z;

	return temp;
}

float3 float3::operator*(const float rhs) const
{
	float3 temp;

	temp.x = x * rhs;
	temp.y = y * rhs;
	temp.z = z * rhs;

	return temp;
}

float3 float3::operator/(const float rhs) const
{
	float3 temp;

	temp.x = x / rhs;
	temp.y = y / rhs;
	temp.z = z / rhs;

	return temp;
}

float3& float3::operator+=(const float3& rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;

	return *this;
}

float3& float3::operator-=(const float3& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;

	return *this;
}

float3& float3::operator*=(const float rhs)
{
	x *= rhs;
	y *= rhs;
	z *= rhs;

	return *this;
}

float3& float3::operator/=(const float rhs)
{
	x /= rhs;
	y /= rhs;
	z /= rhs;

	return *this;
}

bool float3::operator==(const float3& rhs) const
{
	if ((((this->x - rhs.x) < EPSILON) && ((this->x - rhs.x) > -EPSILON)) &&
		(((this->y - rhs.y) < EPSILON) && ((this->x - rhs.x) > -EPSILON)) &&
		(((this->y - rhs.y) < EPSILON) && ((this->x - rhs.x) > -EPSILON)))
		return false;
	else
		return true;
}

bool float3::operator!=(const float3& rhs) const
{
  if ((((this->x - rhs.x) < EPSILON) && ((this->x - rhs.x) > -EPSILON)) &&
    (((this->y - rhs.y) < EPSILON) && ((this->x - rhs.x) > -EPSILON)) &&
    (((this->y - rhs.y) < EPSILON) && ((this->x - rhs.x) > -EPSILON)))
    return true;
  else
    return false;
}

float float3::Dot(const float3& rhs) const
{
	float temp;

	temp = (x * rhs.x) + (y * rhs.y) + (z * rhs.z);

	return temp;
}

float3 float3::Cross(const float3& rhs) const
{
	float3 temp;

	temp.x = (y * rhs.z) - (z * rhs.y);
	temp.y = (z * rhs.x) - (x * rhs.z);
	temp.z = (x * rhs.y) - (y * rhs.x);

	return temp;
}

float float3::Length(void) const
{
	float length = 0.0f;

	length = sqrtf((x * x) + (y * y) + (z * z));

	return length;
}

float float3::LengthSq(void) const
{
	float length = 0.0f;

	length = (x * x) + (y * y) + (z * z);

	return length;
}

float3 float3::Normalize(void)
{
	float norm = 0.0f;
	float3 ret = *this;
	
	norm = sqrtf((x * x) + (y * y) + (z * z));
	
	if (norm)
	{
		ret /= norm;
		/*x = x / norm;
		y = y / norm;
		z = z / norm;*/
	}
	return ret;
}

void float3::Zero(void)
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

void float3::Print(void) const
{
	PrintOut("%5.3f, %5.3f, %5.3f\n", x, y, z);
}


