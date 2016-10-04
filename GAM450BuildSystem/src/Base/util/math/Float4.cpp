/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Float4.cpp 
 * Author: Jorge Antonio Robles
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"

#include "float4.h"
#include <cmath>

static const float EPSILON = 0.0001f;

float4::float4(void) : x(0.0f), y(0.0f), z(0.0f), w(0.0f)
{
}

float4::float4(const float4& rhs) : x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w)
{
}

float4::float4(const float2& rhs, float zz, float ww) : x(rhs.x), y(rhs.y), z(zz), w(ww)
{
}

float4::float4(float xx, const float2& rhs, float ww) : x(xx), y(rhs.x), z(rhs.y), w(ww)
{
}

float4::float4(float xx, float yy, const float2& rhs) : x(xx), y(yy), z(rhs.x), w(rhs.y)
{
}

float4::float4(const float3& rhs, float ww) : x(rhs.x), y(rhs.y), z(rhs.z), w(ww)
{
}

float4::float4(float xx, const float3& rhs) : x(xx), y(rhs.x), z(rhs.y), w(rhs.z)
{
}

float4::float4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w)
{
}


float4& float4::operator=(const float4& rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	w = rhs.w;

	return *this;
}

float4& float4::operator=(const float rhs)
{
	x = rhs;
	y = rhs;
	z = rhs;
	w = rhs;

	return *this;
}

float4 float4::operator-(void) const
{
	float4 temp;

	temp.x = -x;
	temp.y = -y;
	temp.z = -z;
	temp.w = -w;

	return temp;
}

float4 float4::operator+(const float4& rhs) const
{
	float4 temp;

	temp.x = x + rhs.x;
	temp.y = y + rhs.y;
	temp.z = z + rhs.z;
	temp.w = w + rhs.w;

	return temp;
}

float4 float4::operator-(const float4& rhs) const
{
	float4 temp;

	temp.x = x - rhs.x;
	temp.y = y - rhs.y;
	temp.z = z - rhs.z;
	temp.w = w - rhs.w;

	return temp;
}

float4 float4::operator*(const float rhs) const
{
	float4 temp;

	temp.x = x * rhs;
	temp.y = y * rhs;
	temp.z = z * rhs;
	temp.w = w * rhs;

	return temp;
}

float4 float4::operator/(const float rhs) const
{
	float4 temp;

	temp.x = x / rhs;
	temp.y = y / rhs;
	temp.z = z / rhs;
	temp.w = w / rhs;

	return temp;
}

float4& float4::operator+=(const float4& rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	w += rhs.w;

	return *this;
}

float4& float4::operator-=(const float4& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	w -= rhs.w;

	return *this;
}

float4& float4::operator*=(const float rhs)
{
	x *= rhs;
	y *= rhs;
	z *= rhs;
	w *= rhs;

	return *this;
}

float4& float4::operator/=(const float rhs)
{
	x /= rhs;
	y /= rhs;
	z /= rhs;
	w /= rhs;

	return *this;
}

bool float4::operator==(const float4& rhs) const
{
	if ((((this->x - rhs.x) < EPSILON) && ((this->x - rhs.x) > -EPSILON)) &&
		(((this->y - rhs.y) < EPSILON) && ((this->x - rhs.x) > -EPSILON)) &&
		(((this->y - rhs.y) < EPSILON) && ((this->x - rhs.x) > -EPSILON)))
		return true;
	else
		return false;
}

bool float4::operator!=(const float4& rhs) const
{
	if ((((this->x - rhs.x) < EPSILON) && ((this->x - rhs.x) > -EPSILON)) &&
		(((this->y - rhs.y) < EPSILON) && ((this->x - rhs.x) > -EPSILON)) &&
		(((this->y - rhs.y) < EPSILON) && ((this->x - rhs.x) > -EPSILON)))
		return false;
	else
		return true;
}

float float4::Dot(const float4& rhs) const
{
	float temp;

	temp = (x * rhs.x) + (y * rhs.y) + (z * rhs.z);

	return temp;
}

float4 float4::Cross(const float4& rhs) const
{
	float4 temp;

	temp.x = (y * rhs.z) - (z * rhs.y);
	temp.y = (z * rhs.x) - (x * rhs.z);
	temp.z = (x * rhs.y) - (y * rhs.x);

	return temp;
}

float float4::Length(void) const
{
	float length = 0.0f;

	length = sqrtf((x * x) + (y * y) + (z * z) + (w * w));

	return length;
}

float float4::LengthSq(void) const
{
	float length = 0.0f;

	length = (x * x) + (y * y) + (z * z) + (w * w);

	return length;
}

void float4::Normalize(float *pLen)
{
	float norm = 0.0f;

	norm = sqrtf((x * x) + (y * y) + (z * z) + (w * w));

  if (pLen)
    *pLen = norm;

	if (norm)
	{
		x = x / norm;
		y = y / norm;
		z = z / norm;
		w = w / norm;
	}
}

float4 float4::Normal(float *pLen) const
{
  float4 temp(*this);
  temp.Normalize(pLen);
  return temp;
}

void float4::Zero(void)
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 0.0f;
}

float &float4::operator[](const unsigned idx)
{
  return v[idx];
}

float float4::operator[](const unsigned idx) const
{
  return v[idx];
}

void float4::Print(void) const
{
	PrintOut("%5.3f, %5.3f, %5.3f, %5.3f\n", x, y, z, w);
}

float4 float4::Project(const float4& rhs) const
{
  float d = Dot(rhs);
  return rhs * d;
}
