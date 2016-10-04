/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Quaternion.h 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once

#include "base/util/math/float4.h"
#include "base/util/math/float4x4.h"

struct quaternion
{
  // w, x, y, z
  // w is the scalar
  // xyz the axis
  float x;
  float y;
  float z;
  float w;

  quaternion(float w = 1.f, float x = 0.f, float y = 0.f, float z = 0.f);
  quaternion(float4 const &Axis, float fAngle);

  void Set(float w, float x, float y, float z);
  void Set(float4 const &Axis, float fAngle);

  float4 GetAxis(void) const;
  void SetAxis(float4 const &Axis);
  // sets axis to zero and scalar to 1
  void Reset(void);
  // sets all to zero
  void Clear(void);
  // find the angle between two quaternion in radians
  float Angle(quaternion const &b);

  quaternion Normal(void) const;
  quaternion &Normalize(void);

  quaternion &Realign(float4 const &From, float4 const &To);
  quaternion GetInverse(void) const;
  quaternion &Invert(void);
  quaternion GetProjection(quaternion const &q) const;
  quaternion &Project(quaternion const &q);
  quaternion GetConjugate(void) const;
  quaternion &Conjugate(void);
  void Decompose(float &s, float4 &v);

  float Length(void) const;
  float LengthSq(void) const;
  float Dot(quaternion const &q) const;

  float4 RotateVector(float4 const &Axis) const;
  float4 &RotateVectorInPlace(float4 &Axis) const;

  bool operator ==(quaternion const &rhs) const;
  bool operator !=(quaternion const &rhs) const;

  quaternion &operator *=(quaternion const &rhs);
  quaternion &operator +=(quaternion const &rhs);
  quaternion &operator -=(quaternion const &rhs);
  quaternion  operator * (quaternion const &rhs) const;
  quaternion  operator + (quaternion const &rhs) const;
  quaternion  operator - (quaternion const &rhs) const;
  quaternion &operator *=(float rhs);
  quaternion  operator * (float rhs) const;
  friend quaternion operator *(float lhs, quaternion const &rhs);

  float4 operator *(float4 const &rhs) const;
  float4x4 ToMatrix(void) const;

  quaternion Slerp(quaternion const &q, float dt) const;
};