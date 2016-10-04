/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Float4x4.cpp 
 * Author: Jorge Antonio Robles
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"

#include "float4x4.h"
#include "float4.h"
#include "Quaternion.h"
#include "MathUtil.h"
#include "Reals.h"

#include <math.h>

#define NUM_MTX_ELEMENTS 16
#define RAD_TO_DEG (180.0f / PI)

unsigned int Min(unsigned int a, unsigned int b)
{
	return (a < b) ? a : b;
}


float AngleFromXY(float x, float y)
{
	float theta = 0.0f;

	// Quadrant I or IV
	if (x >= 0.0f)
	{
		// If x = 0, then atanf(y/x) = +pi/2 if y > 0
		//                atanf(y/x) = -pi/2 if y < 0
		theta = atanf(y / x); // in [-pi/2, +pi/2]

		if (theta < 0.0f)
			theta += 2.0f*static_cast<float>(PI); // in [0, 2*pi).
	}

	// Quadrant II or III
	else
		theta = atanf(y / x) + static_cast<float>(PI); // in [0, 2*pi).

	return theta;
}

float4 GetColumn(float4x4 const &m, unsigned nIdx)
{
  return float4(m.v[nIdx], m.v[nIdx + 4], m.v[nIdx + 4 * 2], m.v[nIdx + 4 * 3]);
}

float4x4::float4x4(void)
{
	for (int i = 0; i < NUM_MTX_ELEMENTS; ++i)
		v[i] = 0;
}

float4x4::float4x4(const float4x4& rhs)
{
	for (int i = 0; i < NUM_MTX_ELEMENTS; ++i)
		v[i] = rhs.v[i];
}


float4x4::float4x4(float mm00, float mm01, float mm02, float mm03,
	float mm10, float mm11, float mm12, float mm13,
	float mm20, float mm21, float mm22, float mm23,
	float mm30, float mm31, float mm32, float mm33)
{
	m00 = mm00;
	m01 = mm01;
	m02 = mm02;
	m03 = mm03;
	m10 = mm10;
	m11 = mm11;
	m12 = mm12;
	m13 = mm13;
	m20 = mm20;
	m21 = mm21;
	m22 = mm22;
	m23 = mm23;
	m30 = mm30;
	m31 = mm31;
	m32 = mm32;
	m33 = mm33;
}

float4x4::float4x4(float4 const &col0, float4 const &col1, float4 const &col2, float4 const &col3)
{
  m00 = col0.x; m01 = col1.x; m02 = col2.x; m03 = col3.x;
  m10 = col0.y; m11 = col1.y; m12 = col2.y; m13 = col3.y;
  m20 = col0.z; m21 = col1.z; m22 = col2.z; m23 = col3.z;
  m30 = col0.w; m31 = col1.w; m32 = col2.w; m33 = col3.w;
}

float4x4& float4x4::operator=(const float4x4& rhs)
{
	for (int i = 0; i < NUM_MTX_ELEMENTS; ++i)
		v[i] = rhs.v[i];

	return *this;
}


float4x4& float4x4::operator=(float& new_value)
{
	for (int i = 0; i < NUM_MTX_ELEMENTS; ++i)
		v[i] = new_value;

	return *this;
}

float4 float4x4::operator*(const float4& rhs) const
{
	float4 vec;
	float sum = 0.0f;

	for (int i = 0; i < 4; i++)
	{

		for (int k = 0; k < 4; k++)
		{
			sum += m[i][k] * rhs.v[k];
		}

		vec.v[i] = sum;
		sum = 0;
	}


	return vec;
}

float3 float4x4::operator*(const float3& rhs) const
{
	float4 vec;
	float sum = 0.0f;

	for (int i = 0; i < 4; i++)
	{

		for (int k = 0; k < 4; k++)
		{
			sum += m[i][k] * rhs.v[k];
		}

		vec.v[i] = sum;
		sum = 0;
	}

	return vec.xyz;
}

float4x4 float4x4::operator+(const float4x4& rhs) const
{
	float4x4 m;

	for (int i = 0; i < NUM_MTX_ELEMENTS; ++i)
		m.v[i] = v[i] + rhs.v[i];

	return m;
}

float4x4 float4x4::operator-(const float4x4& rhs) const
{
	float4x4 m;

	for (int i = 0; i < NUM_MTX_ELEMENTS; ++i)
		m.v[i] = v[i] - rhs.v[i];

	return m;
}

float4x4 float4x4::operator*(const float4x4& rhs) const
{
	float4x4 temp;

	float sum = 0;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				sum += m[i][k] * rhs.m[k][j];
			}

			temp.m[i][j] = sum;
			sum = 0;
		}
	}

	return temp;
}

float4x4& float4x4::operator+=(const float4x4& rhs)
{
	for (int i = 0; i < NUM_MTX_ELEMENTS; ++i)
		v[i] += rhs.v[i];

	return *this;
}

float4x4& float4x4::operator-=(const float4x4& rhs)
{
	for (int i = 0; i < NUM_MTX_ELEMENTS; ++i)
		v[i] -= rhs.v[i];

	return *this;
}

float4x4& float4x4::operator*=(const float4x4& rhs)
{
	float4x4 temp;

	float sum = 0;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				sum += m[i][k] * rhs.m[k][j];
			}

			temp.m[i][j] = sum;
			sum = 0;
		}
	}

	*this = temp;

	return *this;
}


float4x4 float4x4::operator*(const float rhs) const
{
	float4x4 temp;

	for (int i = 0; i < NUM_MTX_ELEMENTS; ++i)
		temp.v[i] = v[i] * rhs;

	return temp;
}

float4x4 float4x4::operator/(const float rhs) const
{
	float4x4 temp;

	//make sure we're not dividing by 0
	if (rhs)
	{
		for (int i = 0; i < NUM_MTX_ELEMENTS; ++i)
			temp.v[i] = v[i] / rhs;
	}

	return temp;
}

float4x4& float4x4::operator*=(const float rhs)
{
	//float4x4 temp;

	for (int i = 0; i < NUM_MTX_ELEMENTS; ++i)
		v[i] *= rhs;
	//temp.v[i] = v[i]* rhs;

	//*this = temp;

	return *this;
}

float4x4& float4x4::operator/=(const float rhs)
{
	//float4x4 temp;

	//make sure we're not dividing by 0
	if (rhs)
	{
		for (int i = 0; i < NUM_MTX_ELEMENTS; ++i)
			v[i] /= rhs;
		//temp.v[i] = v[i] / rhs;
	}

	//*this = temp;

	return *this;
}

bool float4x4::operator==(const float4x4& rhs) const
{
	for (int i = 0; i < NUM_MTX_ELEMENTS; ++i)
	{
		if ((v[i] - rhs.v[i]) < EPSILON)
			continue;
		else
			return false;
	}
	return true;
}

bool float4x4::operator!=(const float4x4& rhs) const
{
	for (int i = 0; i < NUM_MTX_ELEMENTS; ++i)
	{
		if ((v[i] - rhs.v[i]) < EPSILON)
			continue;
		else
			return true;
	}
	return false;
}

void float4x4::Zero(void)
{
	for (int i = 0; i < NUM_MTX_ELEMENTS; ++i)
		v[i] = 0;
}

void float4x4::Identity(void)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i == j)
			{
				m[i][j] = 1;
			}
			else
			{
				m[i][j] = 0;
			}
		}
	}
}

void float4x4::Rotate(const float3& axis, float radians)
{
	Rotate(axis.x, axis.y, axis.z, radians);
}

void float4x4::Rotate(float x, float y, float z, float radians)
{
	float c0 = Cos(radians);
	float n1C0 = 1.0f - c0;
	float s0 = Sin(radians);

	//| x^2(1-c0)+c0  xy(1-c0)-zs0  xz(1-c0)+ys0 |
	//| xy(1-c0)+zs0  y^2(1-c0)+c0  yz(1-c0)-xs0 |
	//| xz(1-c0)-ys0  yz(1-c0)+xs0  z^2(1-c0)+c0 |
	SetCross(cX, x*x*n1C0 + c0, x*y*n1C0 - z*s0, x*z*n1C0 + y*s0, 0.0f);
	SetCross(cY, x*y*n1C0 + z*s0, y*y*n1C0 + c0, y*z*n1C0 - x*s0, 0.0f);
	SetCross(cZ, x*z*n1C0 - y*s0, y*z*n1C0 + x*s0, z*z*n1C0 + c0, 0.0f);
	SetCross(cW, 0.0f, 0.0f, 0.0f, 1.0f);
}

void float4x4::RotationX(float angle)
{
	Identity();
	angle *= DEG_TO_RAD;
	m[1][1] = cosf(angle);
	m[1][2] = -sinf(angle);
	m[2][1] = sinf(angle);
	m[2][2] = cosf(angle);
}

void float4x4::RotationY(float angle)
{
	Identity();
	angle *= DEG_TO_RAD;
	m[0][0] = cosf(angle);
	m[0][2] = -sinf(angle);
	m[2][0] = sinf(angle);
	m[2][2] = cosf(angle);
}

void float4x4::RotationZ(float angle)
{
	Identity();
	angle *= DEG_TO_RAD;
	m[0][0] = cosf(angle);
	m[0][1] = -sinf(angle);
	m[1][0] = sinf(angle);
	m[1][1] = cosf(angle);
}

void float4x4::SetOrthographicProjection(float w, float h, float zN, float zF)
{
	Identity();
	m00 = 2.0f / w;
	m11 = 2.0f / h;
	m22 = 1.0f / (zN - zF);
	m23 = zN / (zN - zF);
}

float4x4 Transpose(const float4x4& M)
{
	float4x4 ret;

	for (unsigned i = 0; i < 4; ++i)
	{
		for (unsigned j = 0; j < 4; ++j)
		{
			ret.m[i][j] = M.m[j][i];
		}
	}

	return ret;
}

float4x4 float4x4::RotationMatrix(float x, float y, float z)
{
	float4x4 rotation, rotX, rotY, rotZ;

	rotX.RotationX(x);

	rotY.RotationY(y);

	rotZ.RotationZ(z);

	rotation = rotX * rotY * rotZ;

	return rotation;
}

float4x4 float4x4::RotationMatrix(const float3& rotation)
{
  return RotationMatrix(rotation.x, rotation.y, rotation.z);
}

float4x4 float4x4::ScaleMatrix(const float3& scale)
{
	float4x4 temp;
	
	temp.m00 = scale.x;
	temp.m11 = scale.y;
	temp.m22 = scale.z;
	temp.m33 = 1.0f;

	return temp;
}

float4x4 float4x4::TranslationMatrix(const float3& translation)
{
	float4x4 temp;
	temp.Identity();

	temp.m[3][0] = translation.x;
	temp.m[3][1] = translation.y;
	temp.m[3][2] = translation.z;
	temp.m[3][3] = 1.f;

	return temp;
}

float4x4 TensorProduct(const float4& U, const float4& V)
{
	float4x4 tensor;
	tensor.Identity();

	tensor.m[0][0] = U.x * V.x;
	tensor.m[0][1] = U.x * V.y;
	tensor.m[0][2] = U.x * V.z;

	tensor.m[1][0] = U.y * V.x;
	tensor.m[1][1] = U.y * V.y;
	tensor.m[1][2] = U.y * V.z;

	tensor.m[2][0] = U.z * V.x;
	tensor.m[2][1] = U.z * V.y;
	tensor.m[2][2] = U.z * V.z;

	return tensor;
}

float4x4 TildeProduct(const float4& U)
{
	float4x4 tilde;
	tilde.Identity();

	tilde.m[0][0] = 0.0f;
	tilde.m[0][1] = -U.z;
	tilde.m[0][2] = U.y;

	tilde.m[1][0] = U.z;
	tilde.m[1][1] = 0.0f;
	tilde.m[1][2] = -U.x;

	tilde.m[2][0] = -U.y;
	tilde.m[2][1] = U.x;
	tilde.m[2][2] = 0.0f;

	return tilde;
}

float4x4 AxisRotation(const float4& axis, const float& angle)
{
	float4x4 rotation;
	rotation.Identity();

	rotation *= cosf(angle);

	rotation += TensorProduct(axis, axis) * (1.0f - cosf(angle));

	rotation += TildeProduct(axis) * sinf(angle);

	return rotation;
}

void RotateVectorByAxis(float4& U, float4& V, float4& axis, float angle)
{
	float4x4 RotateByAxis;
	RotateByAxis = Transpose(AxisRotation(axis, angle));
	//RotateByAxis = AxisRotation(axis, angle);

	U = RotateByAxis * U;
	U.Normalize();

	V = RotateByAxis * V;
	V.Normalize();
}

void float4x4::Print(void) const
{
	PrintOut("--------------------------\n");
	PrintOut("%5.3f %5.3f %5.3f %5.3f\n", m00, m01, m02, m03);
	PrintOut("%5.3f %5.3f %5.3f %5.3f\n", m10, m11, m12, m13);
	PrintOut("%5.3f %5.3f %5.3f %5.3f\n", m20, m21, m22, m23);
	PrintOut("%5.3f %5.3f %5.3f %5.3f\n", m30, m31, m32, m33);
	PrintOut("--------------------------\n");
}

float4x4 float4x4::Transpose(void) const
{
  float4x4 result = *this;

  // hardcode the transpose
  std::swap(result.m[0][1], result.m[1][0]);
  std::swap(result.m[0][2], result.m[2][0]);
  std::swap(result.m[0][3], result.m[3][0]);
  std::swap(result.m[1][2], result.m[2][1]);
  std::swap(result.m[1][3], result.m[3][1]);
  std::swap(result.m[2][3], result.m[3][2]);

  return result;
}


float4x4 float4x4::CreateRotationMatrix(float x, float y, float z)
{
  float4x4 rotation, rotX, rotY, rotZ;

  rotX.RotationX(x);
  rotY.RotationY(y);
  rotZ.RotationZ(z);

  rotation = rotX * rotY * rotZ;

  return rotation;
}

float4x4 float4x4::CreateScaleMatrix(const float3& scale)
{
  float4x4 temp;
  temp.Identity();

  temp.m00 = scale.x;
  temp.m11 = scale.y;
  temp.m22 = scale.z;
  temp.m33 = 1.0f;

  return temp;
}

float4x4 float4x4::CreateTranslationMatrix(const float3& translation)
{
  float4x4 temp;
  temp.Identity();

  temp.m[0][3] = translation.x;
  temp.m[1][3] = translation.y;
  temp.m[2][3] = translation.z;
  temp.m[3][3] = 1.f;

  return temp;
}

// REFERENCE: http://www.cg.info.hiroshima-cu.ac.jp/~miyazaki/knowledge/teche52.html
quaternion float4x4::ToQuaternion(void) const
{
  float q0 = ( m00 + m11 + m22 + 1.f) / 4.f;
  float q1 = ( m00 - m11 - m22 + 1.f) / 4.f;
  float q2 = (-m00 + m11 - m22 + 1.f) / 4.f;
  float q3 = (-m00 - m11 + m22 + 1.f) / 4.f;

  if (q0 < 0.f)
    q0 = 0.f;
  if (q1 < 0.f)
    q1 = 0.f;
  if (q2 < 0.f)
    q2 = 0.f;
  if (q3 < 0.f)
    q3 = 0.f;

  q0 = std::sqrt(q0);
  q1 = std::sqrt(q1);
  q2 = std::sqrt(q2);
  q3 = std::sqrt(q3);

  if (q0 >= q1 && q0 >= q2 && q0 >= q3)
  {
    q0 *= 1.f;
    q1 *= Sign(m21 - m12);
    q2 *= Sign(m02 - m20);
    q3 *= Sign(m10 - m01);
  }
  else if (q1 >= q0 && q1 >= q2 && q1 >= q3)
  {
    q0 *= Sign(m21 - m12);
    q1 *= 1.f;
    q2 *= Sign(m10 + m01);
    q3 *= Sign(m02 + m20);
  }
  else if (q2 >= q0 && q2 >= q1 && q2 >= q3)
  {
    q0 *= Sign(m02 - m20);
    q1 *= Sign(m10 + m01);
    q2 *= 1.f;
    q3 *= Sign(m21 + m12);
  }
  else if (q3 >= q0 && q3 >= q1 && q3 >= q2)
  {
    q0 *= Sign(m10 - m01);
    q1 *= Sign(m20 + m02);
    q2 *= Sign(m21 + m12);
    q3 *= 1.f;
  }
  else
    printf("Error");

  float r = std::sqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
  q0 /= r;
  q1 /= r;
  q2 /= r;
  q3 /= r;

  return quaternion(q0, q1, q2, q3);
}

float4x4 float4x4::GetInverse(void) const
{
  float4x4 result = *this;
  result.ToInverse();
  return result;
}

float4x4 &float4x4::ToInverse(void)
{
  float det = m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2]) -
              m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
              m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);

  if (Equals(det, 0.f))
    return *this;

  det = 1.f / det;

  float4x4 result; result.Identity();
  result.m[0][0] = (m[1][1] * m[2][2] - m[2][1] * m[1][2]) * det;
  result.m[0][1] = (m[0][2] * m[2][1] - m[0][1] * m[2][2]) * det;
  result.m[0][2] = (m[0][1] * m[1][2] - m[0][2] * m[1][1]) * det;
  result.m[1][0] = (m[1][2] * m[2][0] - m[1][0] * m[2][2]) * det;
  result.m[1][1] = (m[0][0] * m[2][2] - m[0][2] * m[2][0]) * det;
  result.m[1][2] = (m[1][0] * m[0][2] - m[0][0] * m[1][2]) * det;
  result.m[2][0] = (m[1][0] * m[2][1] - m[2][0] * m[1][1]) * det;
  result.m[2][1] = (m[2][0] * m[0][1] - m[0][0] * m[2][1]) * det;
  result.m[2][2] = (m[0][0] * m[1][1] - m[1][0] * m[0][1]) * det;


  *this = result;

  return *this;
  //float det = GetDeterminant();
  //// if the determinent is zero then it has no inverse
  //if (Equals(det, 0.f))
  //  return *this;
  //
  //float4x4 identity;
  //identity.Identity();
  //
  //GaussJordanElimination(identity, *this);
  //
  //return *this;
}

float float4x4::GetDeterminant(void) const
{
  float det(1.f);
  float flip(1.f);
  float mult(1.f);

  float const r = 4;
  float const c = 4;

  float4x4 mat = *this;

  unsigned i = 0;
  while (i < r)
  {
    // if it's zero, swap with the row underneath it
    if (mat.m[i][i] == 0)
    {
      if (i != r - 1 && i != c - 1)
      {
        mat.SwapRows(i, i + 1);
        flip *= -flip;
      }
    }

    // reduce the element to one
    if (mat.m[i][i] != 1.f && mat.m[i][i] != 0.f)
    {
      mult *= mat.m[i][i];
      mat.ScaleRow(i, 1.f / mat.m[i][i]);
    }

    if (i == r - 1 || i == c - 1)
      break;

    // reduce each element to zero underneath
    for (unsigned j = i; j < r - 1; ++j)
      mat.ScaleAdd(j + 1, i, -1.f * mat.m[j + 1][i]);

    ++i;
  }

  // multiply the diagonal's together to grab the determinent
  for (unsigned j = 0; j < r; ++j)
    det *= mat.m[j][j];

  // flip the sign if I need to
  det *= flip;
  det *= mult;

  return det;
}

void float4x4::SwapRows(unsigned row1, unsigned row2)
{
  for (unsigned nIdx = 0; nIdx < 4; ++nIdx)
    std::swap(m[row1][nIdx], m[row2][nIdx]);
}

void float4x4::ScaleRow(unsigned row, float s)
{
  for (unsigned nIdx = 0; nIdx < 4; ++nIdx)
    m[row][nIdx] *= s;
}

void float4x4::AddRow(unsigned row1, unsigned row2)
{
  for (unsigned nIdx = 0; nIdx < 4; ++nIdx)
    m[row1][nIdx] += m[row2][nIdx];
}

void float4x4::ScaleAdd(unsigned row1, unsigned row2, float s)
{
  for (unsigned nIdx = 0; nIdx < 4; ++nIdx)
  {
    float a = m[row2][nIdx];
    a *= s;

    m[row1][nIdx] += a;
  }
}

void float4x4::GaussJordanElimination(float4x4 &lhs, float4x4 &rhs)
{
  int const r = 8;
  int const size = 4;

  GaussElimination(lhs, rhs);

  int i = size - 1;

  while (i >= 0)
  {
    // reduce up to make it zero
    for (int j = i; j > 0; --j)
    {
      lhs.ScaleAdd(j - 1, i, -1.f * lhs.m[j - 1][i]);
      rhs.ScaleAdd(j - 1, i, -1.f * lhs.m[j - 1][i]);
    }

    --i;
  }
}

void float4x4::GaussElimination(float4x4 &lhs, float4x4 &rhs)
{
  unsigned const r = 4;
  unsigned const c = 8;

  unsigned i = 0;

  while (i < r && i < c)
  {
    if (Equals(lhs.m[i][i], 0.f))
    {
      if (i != r - 1 && i != c - 1)
      {
        lhs.SwapRows(i, i + 1);
        rhs.SwapRows(i, i + 1);
      }
    }

    // reduce the element to one
    if (!Equals(lhs.m[i][i], 1.f) && !Equals(lhs.m[i][i], 0.f))
    {
      lhs.ScaleRow(i, 1.f / lhs.m[i][i]);
      rhs.ScaleRow(i, 1.f / lhs.m[i][i]);
    }

    if (i == r - 1 || i == c - 1)
      return;

    // then reduce each element to zero underneath
    for (unsigned j = i; j < r - 1; ++j)
    {
      lhs.ScaleAdd(j + 1, i, -1.f * lhs.m[j + 1][i]);
      rhs.ScaleAdd(j + 1, i, -1.f * lhs.m[j + 1][i]);
    }

    ++i;
  }
}

void float4x4::SetCross(unsigned index, float4& crossVector)
{

	SetCross(index, crossVector.x, crossVector.y, crossVector.z, crossVector.w);
}
void float4x4::SetCross(unsigned index, float3& crossVector3, float w)
{

	SetCross(index, crossVector3.x, crossVector3.y, crossVector3.z, w);
}
void float4x4::SetCross(unsigned index, float x, float y, float z, float w)
{
	float4x4& self = *this;
	if (index > 3)
		ErrorIf(index > 3, "Matrix4 - Index out of range.");
#ifdef ColumnBasis
	self[index].Set(x, y, z, w);
#else
	v[index] = x;
	v[4 + index] = y;
	v[8 + index] = z;
	v[12 + index] = w;
#endif
}

//float3 float4x4::TransformNormal(float4x4& matrix, float3& normal)
//{
//	float x = (*(float3*)&matrix[0]).Dot(normal);
//	float y = (*(float3*)&matrix[1]).Dot(normal);
//	float z = (*(float3*)&matrix[2]).Dot(normal);
//	return float3(x, y, z);
//}
