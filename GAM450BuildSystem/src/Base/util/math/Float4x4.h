/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Float4x4.h
 * Author: Jorge Antonio Robles
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once

#include<DirectXMath.h>

class float4;
class float3;
struct quaternion;


unsigned int Min(unsigned int a, unsigned int b);
float AngleFromXY(float, float);
void RotateVectorByAxis(float4& U, float4& V, float4& axis, float angle);

struct float4x4
{
	union
	{
		struct
		{
			float m00, m01, m02, m03,
			m10, m11, m12, m13,
			m20, m21, m22, m23,
			m30, m31, m32, m33;
		};

		float m[4][4];
		float v[16];
	};

	/*
	This union lets us access the data in multiple ways
	All of these are modifying the same location in memory

	float4x4 mtx;
	mtx.m[2][2] = 1.0f;
	mtx.v[10] = 2.0f;
	mtx.m22 = 3.0f;
	*/

	float4x4(void);

	float4x4(const float4x4& rhs);

	float4x4(float mm00, float mm01, float mm02, float mm03,
		float mm10, float mm11, float mm12, float mm13,
		float mm20, float mm21, float mm22, float mm23,
		float mm30, float mm31, float mm32, float mm33);

	float4x4(float4 const &col0, float4 const &col1, float4 const &col2, float4 const &col3);
	float4x4& float4x4::operator=(float& new_value);

	float4x4& operator=(const float4x4& rhs);

	float4 operator*(const float4& rhs) const;
	float3 operator*(const float3& rhs) const;

	float4x4 operator+(const float4x4& rhs) const;
	float4x4 operator-(const float4x4& rhs) const;
	float4x4 operator*(const float4x4& rhs) const;

	float4x4& operator+=(const float4x4& rhs);
	float4x4& operator-=(const float4x4& rhs);
	float4x4& operator*=(const float4x4& rhs);

	float4x4 operator*(const float rhs) const;
	float4x4 operator/(const float rhs) const;

	float4x4& operator*=(const float rhs);
	float4x4& operator/=(const float rhs);

	bool operator==(const float4x4& rhs) const;
	bool operator!=(const float4x4& rhs) const;

	void Zero(void);

	void Identity(void);

	void Rotate(const float3& axis, float radians);

	void Rotate(float x, float y, float z, float radians);

	void RotationX(float angle);

	void RotationY(float angle);

	void RotationZ(float angle);

	void SetOrthographicProjection(float w, float h, float zN, float zF);

	float4x4 RotationMatrix(float x, float y, float z);
	float4x4 RotationMatrix(const float3& rotation);
	static float4x4 CreateRotationMatrix(float x, float y, float z);
	float4x4 ScaleMatrix(const float3& scale);
	static float4x4 CreateScaleMatrix(const float3& scale);
	float4x4 TranslationMatrix(const float3& translation);
	static float4x4 CreateTranslationMatrix(const float3& translation);
	float4x4 Transpose(void) const;

	float4x4 GetInverse(void) const;
	float4x4 &ToInverse(void);
	float GetDeterminant(void) const;

	// lhs is identity, rhs is original matrix after function call,
	// rhs should be identity and lhs should hold inverse
	static void GaussJordanElimination(float4x4 &lhs, float4x4 &rhs);
	static void GaussElimination(float4x4 &lhs, float4x4 &rhs);

	void SwapRows(unsigned row1, unsigned row2);
	void ScaleRow(unsigned row, float s);
	void AddRow(unsigned row1, unsigned row2);
	void ScaleAdd(unsigned row1, unsigned row2, float s);

	// Already implemented, simple print function
	void Print(void) const;

	// convert this matrix into a quaternion
	quaternion ToQuaternion(void) const;


	void SetCross(unsigned index, float4& crossVector);
	void SetCross(unsigned index, float3& crossVector3, float w);
	void SetCross(unsigned index, float x, float y, float z, float w);

	//float4& operator[](unsigned index){ return ((float4*)this)[index]; }
	//const float4& operator[](unsigned index)const{ return ((float4*)this)[index]; }

	//float3 TransformNormal(float4x4& matrix, float3& normal);

};

float4x4 Transpose(const float4x4& M);
float4x4 AxisRotation(const float4& axis, const float& angle);
float4x4 TensorProduct(const float4& U, const float4& V);
float4x4 TildeProduct(const float4& U);
float4 GetColumn(float4x4 const &basis, unsigned nIdx);