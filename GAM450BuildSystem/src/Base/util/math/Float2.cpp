/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Float2.cpp 
 * Author: Jorge Antonio Robles
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"
#include "Float2.h"
#include <cmath>

static const float EPSILON = 0.0001f;

float2::float2(void) : x(0.0f), y(0.0f)
{

}

float2::float2(const float2& rhs) : x(rhs.x), y(rhs.y)
{
}

float2::float2(float xx, float yy) : x(xx), y(yy)
{
}

float2& float2::operator=(const float2& rhs)
{
	x = rhs.x;
	y = rhs.y;
	
	return *this;
}

float2 float2::operator-(void) const
{
	float2 temp;

	temp.x = -x;
	temp.y = -y;

	return temp;
}

float2 float2::operator+(const float2& rhs) const
{
	float2 temp;

	temp.x = x + rhs.x;
	temp.y = y + rhs.y;

	return temp;
}

float2 float2::operator-(const float2& rhs) const
{
	float2 temp;

	temp.x = x - rhs.x;
	temp.y = y - rhs.y;

	return temp;
}

float2 float2::operator*(const float rhs) const
{
	float2 temp;

	temp.x = x * rhs;
	temp.y = y * rhs;
	
	return temp;
}

float2 float2::operator/(const float rhs) const
{
	float2 temp;

	temp.x = x / rhs;
	temp.y = y / rhs;

	return temp;
}

float2& float2::operator+=(const float2& rhs)
{
	x += rhs.x;
	y += rhs.y;

	return *this;
}

float2& float2::operator-=(const float2& rhs)
{
	x -= rhs.x;
	y -= rhs.y;

	return *this;
}

float2& float2::operator*=(const float rhs)
{
	x *= rhs;
	y *= rhs;

	return *this;
}

float2& float2::operator/=(const float rhs)
{
	x /= rhs;
	y /= rhs;

	return *this;
}

bool float2::operator==(const float2& rhs) const
{
	if ((((this->x - rhs.x) < EPSILON) && ((this->x - rhs.x) > -EPSILON)) &&
		(((this->y - rhs.y) < EPSILON) && ((this->x - rhs.x) > -EPSILON)))
		return true;
	else
		return false;
}

bool float2::operator!=(const float2& rhs) const
{
	if ((((this->x - rhs.x) < EPSILON) && ((this->x - rhs.x) > -EPSILON)) &&
		(((this->y - rhs.y) < EPSILON) && ((this->x - rhs.x) > -EPSILON)))
		return false;
	else
		return true;
}

float float2::Dot(const float2& rhs) const
{
	float temp;

	temp = (x * rhs.x) + (y * rhs.y);

	return temp;
}

float float2::Length(void) const
{
	float length = 0.0f;

	length = sqrtf((x * x) + (y * y));

	return length;
}

float float2::LengthSq(void) const
{
	float length = 0.0f;

	length = (x * x) + (y * y);

	return length;
}

void float2::Normalize(void)
{
	float norm = 0.0f;

	norm = sqrtf((x * x) + (y * y));

	if (norm)
	{
		x = x / norm;
		y = y / norm;
	}
}

void float2::Zero(void)
{
	x = 0.0f;
	y = 0.0f;
}

void float2::Print(void) const
{
	PrintOut("%5.3f, %5.3f\n", x, y);
}


