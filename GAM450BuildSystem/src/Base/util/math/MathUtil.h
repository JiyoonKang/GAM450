/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: MathUtil.h 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once

#define PI 3.1415926535897932384626433832795f
#define EPSILON 0.0001f
#define DEG_TO_RAD (PI / 180.0f)

#include <algorithm>

template <typename T>
T Clamp(T const &x, T const &nMin, T const &nMax)
{
  return std::min<T>(nMax, std::max<T>(nMin, x));
}

template <typename T>
bool Equals(T left, T right)
{
  return std::abs(left - right) < EPSILON;
}

template <typename T>
bool EqualsEp(T left, T right, T epsilon)
{
  return std::abs(left - right) < epsilon;
}

template <typename T>
T Sign(T const &x)
{
  return x >= T() ? static_cast<T>(1) : -static_cast<T>(1);
}