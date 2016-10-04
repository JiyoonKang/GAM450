/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Quaternion.cpp 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"
#include "base/util/math/Quaternion.h"
#include "base/util/math/MathUtil.h"

quaternion::quaternion(float _w /* = 1.f */, float _x /* = 0.f */, float _y /* = 0.f */, float _z /* = 0.f */)
  : w(_w)
  , x(_x)
  , y(_y)
  , z(_z)
{

}

quaternion::quaternion(float4 const &Axis, float fAngle)
{
  w = std::cos(0.5f * fAngle);
  float s = std::sin(0.5f * fAngle);
  x = Axis.x * s;
  y = Axis.y * s;
  z = Axis.z * s;
}

void quaternion::Set(float _w, float _x, float _y, float _z)
{
  w = _w;
  x = _x;
  y = _y;
  z = _z;
}

void quaternion::Set(float4 const &Axis, float fAngle)
{
  w = std::cos(0.5f * fAngle);
  float s = std::sin(0.5f * fAngle);
  x = Axis.x * s;
  y = Axis.y * s;
  z = Axis.z * s;
}

float4 quaternion::GetAxis(void) const
{
  return float4(x, y, z, 0.f);
}

void quaternion::SetAxis(float4 const &Axis)
{
  x = Axis.x;
  y = Axis.y;
  z = Axis.z;
}

void quaternion::Reset(void)
{
  Set(1.f, 0.f, 0.f, 0.f);
}

void quaternion::Clear(void)
{
  Set(0.f, 0.f, 0.f, 0.f);
}

float quaternion::Angle(quaternion const &b)
{
  quaternion inv = GetInverse();
  quaternion res = b * inv;
  float fAngle = std::acosf(res.w) * 2.f;
  if (fAngle > PI)
    fAngle = 2 * PI - fAngle;

  return fAngle;
}

quaternion quaternion::Normal(void) const
{
  quaternion result = *this;
  result.Normalize();
  return result;
}

quaternion &quaternion::Normalize(void)
{
  float fInv = 1.f / Length();
  w *= fInv;
  x *= fInv;
  y *= fInv;
  z *= fInv;
  return *this;
}

quaternion &quaternion::Realign(float4 const &From, float4 const &To)
{
  float4 ToCopy = To;
  float4 FromCopy = From;
  ToCopy.Normalize();
  FromCopy.Normalize();

  float4 Axis = FromCopy.Cross(ToCopy);
  float fScalar = std::sqrt(ToCopy.LengthSq() * FromCopy.LengthSq()) + ToCopy.Dot(FromCopy);

  Set(fScalar, Axis.x, Axis.y, Axis.z);
  Normalize();

  return *this;
}

quaternion quaternion::GetInverse(void) const
{
  quaternion result = GetConjugate();

  float fLenSq = result.LengthSq();
  if (Equals(fLenSq, 1.f) || Equals(fLenSq, 0.f))
    return result;

  return result *= (1.f / std::sqrt(fLenSq));
}

quaternion &quaternion::Invert(void)
{
  Conjugate();

  float fLenSq = LengthSq();
  if (Equals(fLenSq, 1.f) || Equals(fLenSq, 0.f))
    return *this;

  *this *= (1.f / fLenSq);

  return *this;
}

quaternion quaternion::GetProjection(quaternion const &q) const
{
  quaternion result = *this;
  result.Project(q);
  return result;
}

quaternion &quaternion::Project(quaternion const &q)
{
  float d = Dot(q);
  *this = q * d;

  return *this;
}

float quaternion::Length(void) const
{
  return std::sqrt(LengthSq());
}

float quaternion::LengthSq(void) const
{
  return w * w + x * x + y * y + z * z;
}

float quaternion::Dot(quaternion const &q) const
{
  return w * q.w + x * q.x + y * q.y + z * q.z;
}

float4 quaternion::RotateVector(float4 const &Axis) const
{
  quaternion result = (*this) * quaternion(0.f, Axis.x, Axis.y, Axis.z) * GetInverse();
  return result.GetAxis();
}

float4 &quaternion::RotateVectorInPlace(float4 &Axis) const
{
  Axis = RotateVector(Axis);
  return Axis;
}

bool quaternion::operator ==(quaternion const &rhs) const
{
  return Equals(x, rhs.x) && Equals(y, rhs.y) && Equals(z, rhs.z) && Equals(w, rhs.w);
}

bool quaternion::operator !=(quaternion const &rhs) const
{
  return !Equals(x, rhs.x) || !Equals(y, rhs.y) || !Equals(z, rhs.z) || !Equals(w, rhs.w);
}

quaternion &quaternion::operator *=(quaternion const &rhs)
{
  float fScalar = w;
  float4 Axis = GetAxis();

  w = fScalar * rhs.w - Axis.Dot(rhs.GetAxis());
  SetAxis(rhs.GetAxis() * fScalar + Axis * rhs.w + Axis.Cross(rhs.GetAxis()));

  return *this;
}

quaternion &quaternion::operator +=(quaternion const &rhs)
{
  w += rhs.w;
  x += rhs.x;
  y += rhs.y;
  z += rhs.z;

  return *this;
}

quaternion &quaternion::operator -=(quaternion const &rhs)
{
  w -= rhs.w;
  x -= rhs.x;
  y -= rhs.y;
  z -= rhs.z;

  return *this;
}

quaternion quaternion::operator *(quaternion const &rhs) const
{
  quaternion result = *this;
  result *= rhs;
  return result;
}

quaternion quaternion::operator +(quaternion const &rhs) const
{
  quaternion result = *this;
  result += rhs;
  return result;
}

quaternion quaternion::operator -(quaternion const &rhs) const
{
  quaternion result = *this;
  result -= rhs;
  return result;
}

quaternion &quaternion::operator *=(float rhs)
{
  w *= rhs;
  x *= rhs;
  y *= rhs;
  z *= rhs;

  return *this;
}

quaternion quaternion::operator *(float rhs) const
{
  quaternion result = *this;
  result *= rhs;
  return result;
}

quaternion operator *(float lhs, quaternion const &rhs)
{
  return rhs * lhs;
}

float4 quaternion::operator *(float4 const &rhs) const
{
  return RotateVector(rhs);
}

float4x4 quaternion::ToMatrix(void) const
{
  float xx = x * x;
  float yy = y * y;
  float zz = z * z;

  float4x4 quat;

  quat.m00 = 1.f - 2.f*yy - 2.f*zz;     quat.m01 = 2.f*x*y - 2.f*z*w;     quat.m02 = 2.f*x*z + 2.f*y*w; quat.m03 = 0.f;
  quat.m10 =     2.f*x*y + 2.f*z*w; quat.m11 = 1.f - 2.f*xx - 2.f*zz;     quat.m12 = 2.f*y*z - 2.f*x*w; quat.m13 = 0.f;
  quat.m20 =     2.f*x*z - 2.f*y*w;     quat.m21 = 2.f*y*z + 2.f*x*w; quat.m22 = 1.f - 2.f*xx - 2.f*yy; quat.m23 = 0.f;
  quat.m30 =                   0.f;                   quat.m31 = 0.f;                   quat.m32 = 0.f; quat.m33 = 1.f;

  return quat;
}

quaternion quaternion::GetConjugate(void) const
{
  return quaternion(w, -x, -y, -z);
}

quaternion& quaternion::Conjugate(void)
{
  x = -x;
  y = -y;
  z = -z;

  return *this;
}

quaternion quaternion::Slerp(quaternion const &q, float dt) const
{
  quaternion const &p = *this;
  float cos = p.Dot(q);

  // flip if needed
  quaternion q1 = q;
  if (cos < 0.f)
  {
    cos = -cos;
    q1.Set(-q.w, -q.x, -q.y, -q.z);
  }

  float const theta = std::acos(Clamp(cos, -1.f, 1.f));
  if (Equals(theta, 0.f))
    return p;

  float const invSin = 1.f / std::sin(theta);
  float const sin1 = std::sin((1.f - dt) * theta) * invSin;
  float const sin2 = std::sin(dt * theta) * invSin;

  return sin1 * p + sin2 * q1;
}

void quaternion::Decompose(float &s, float4 &v)
{
  s = w;

  float theta = 2.f * std::acos(s);
  float4 axis = GetAxis();
  v = axis * std::sin(theta / 2.f);
}