/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Float4.h 
 * Author: Jorge Antonio Robles
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once

#include "Base/util/math/float3.h"

class float4
{
private:

	template<int a, int b, int c, int d>
	class swizzle
	{
	public:

		operator float4(void)
		{
			return float4(v[a], v[b], v[c], v[d]);
		}

		operator float3(void)
		{
			return float3(v[a], v[b], v[c]);
		}

		operator float2(void)
		{
			return float2(v[a], v[b]);
		}


		float4& operator=(const float4& rhs)
		{
			ErrorIf((a == b || a == c || a == d || b == c || b == d), "Cannot use this swizzle for that operation");

			v[a] = rhs.x;
			v[b] = rhs.y;
			v[c] = rhs.z;
			v[d] = rhs.w;

			return *(float4*)(this);
		}

		float4 operator-(void)
		{
			return float4(-v[a], -v[b], -v[c], -v[d]);
		}

		float4 operator+(const float4& rhs) const
		{
			return float4(v[a] + rhs.x, v[b] + rhs.y, v[c] + rhs.z, v[d] + rhs.w);
		}

		float4 operator-(const float4& rhs) const
		{
			return float4(v[a] - rhs.x, v[b] - rhs.y, v[c] - rhs.z, v[d] - rhs.w);
		}

		float4 operator*(const float rhs) const
		{
			return float4(v[a] * rhs, v[b] * rhs, v[c] * rhs, v[d] * rhs);
		}

		float4 operator/(const float rhs) const
		{
			return float4(v[a] / rhs, v[b] / rhs, v[c] / rhs, v[d] / rhs);
		}

		float4& operator+=(const float4& rhs)
		{
			ErrorIf((a == b || a == c || a == d || b == c || b == d), "Cannot use this swizzle for that operation");

			v[a] += rhs.x;
			v[b] += rhs.y;
			v[c] += rhs.z;
			v[d] += rhs.w;

			return *(float4*)(this);
		}

		float4& operator-=(const float4& rhs)
		{
			ErrorIf((a == b || a == c || a == d || b == c || b == d), "Cannot use this swizzle for that operation");

			v[a] -= rhs.x;
			v[b] -= rhs.y;
			v[c] -= rhs.z;
			v[d] -= rhs.w;

			return *(float4*)(this);
		}

		float4& operator*=(const float rhs)
		{
			ErrorIf((a == b || a == c || a == d || b == c || b == d), "Cannot use this swizzle for that operation");

			v[a] *= rhs;
			v[b] *= rhs;
			v[c] *= rhs;
			v[d] *= rhs;

			return *(float4*)(this);
		}

		float4& operator/=(const float rhs)
		{
			ErrorIf((a == b || a == c || a == d || b == c || b == d), "Cannot use this swizzle for that operation");

			v[a] /= rhs;
			v[b] /= rhs;
			v[c] /= rhs;
			v[d] /= rhs;

			return *(float4*)(this);
		}


	private:
		float v[4];

	};

public:


	//This is done so that the vector
	//can be accessed in multiple ways.
	union
	{
		struct
		{
			float x, y, z, w;
		};

		//float2 variations
		swizzle<0, 0, -1, -1> xx;
		swizzle<0, 1, -1, -1> xy;
		swizzle<0, 2, -1, -1> xz;
		swizzle<0, 3, -1, -1> xw;
		swizzle<1, 0, -1, -1> yx;
		swizzle<1, 1, -1, -1> yy;
		swizzle<1, 2, -1, -1> yz;
		swizzle<1, 3, -1, -1> yw;
		swizzle<2, 0, -1, -1> zx;
		swizzle<2, 1, -1, -1> zy;
		swizzle<2, 2, -1, -1> zz;
		swizzle<2, 3, -1, -1> zw;
		swizzle<3, 0, -1, -1> wx;
		swizzle<3, 1, -1, -1> wy;
		swizzle<3, 2, -1, -1> wz;
		swizzle<3, 3, -1, -1> ww;

		//float3 variations
		swizzle<0, 0, 0, -1> xxx;
		swizzle<0, 0, 1, -1> xxy;
		swizzle<0, 0, 2, -1> xxz;
		swizzle<0, 0, 3, -1> xxw;
		swizzle<0, 1, 0, -1> xyx;
		swizzle<0, 1, 1, -1> xyy;
		swizzle<0, 1, 2, -1> xyz;
		swizzle<0, 1, 3, -1> xyw;
		swizzle<0, 2, 0, -1> xzx;
		swizzle<0, 2, 1, -1> xzy;
		swizzle<0, 2, 2, -1> xzz;
		swizzle<0, 2, 3, -1> xzw;
		swizzle<0, 3, 0, -1> xwx;
		swizzle<0, 3, 1, -1> xwy;
		swizzle<0, 3, 2, -1> xwz;
		swizzle<0, 3, 3, -1> xww;
		swizzle<1, 0, 0, -1> yxx;
		swizzle<1, 0, 1, -1> yxy;
		swizzle<1, 0, 2, -1> yxz;
		swizzle<1, 0, 3, -1> yxw;
		swizzle<1, 1, 0, -1> yyx;
		swizzle<1, 1, 1, -1> yyy;
		swizzle<1, 1, 2, -1> yyz;
		swizzle<1, 1, 3, -1> yyw;
		swizzle<1, 2, 0, -1> yzx;
		swizzle<1, 2, 1, -1> yzy;
		swizzle<1, 2, 2, -1> yzz;
		swizzle<1, 2, 3, -1> yzw;
		swizzle<1, 3, 0, -1> ywx;
		swizzle<1, 3, 1, -1> ywy;
		swizzle<1, 3, 2, -1> ywz;
		swizzle<1, 3, 3, -1> yww;
		swizzle<2, 0, 0, -1> zxx;
		swizzle<2, 0, 1, -1> zxy;
		swizzle<2, 0, 2, -1> zxz;
		swizzle<2, 0, 3, -1> zxw;
		swizzle<2, 1, 0, -1> zyx;
		swizzle<2, 1, 1, -1> zyy;
		swizzle<2, 1, 2, -1> zyz;
		swizzle<2, 1, 3, -1> zyw;
		swizzle<2, 2, 0, -1> zzx;
		swizzle<2, 2, 1, -1> zzy;
		swizzle<2, 2, 2, -1> zzz;
		swizzle<2, 2, 3, -1> zzw;
		swizzle<2, 3, 0, -1> zwx;
		swizzle<2, 3, 1, -1> zwy;
		swizzle<2, 3, 2, -1> zwz;
		swizzle<2, 3, 3, -1> zww;
		swizzle<3, 0, 0, -1> wxx;
		swizzle<3, 0, 1, -1> wxy;
		swizzle<3, 0, 2, -1> wxz;
		swizzle<3, 0, 3, -1> wxw;
		swizzle<3, 1, 0, -1> wyx;
		swizzle<3, 1, 1, -1> wyy;
		swizzle<3, 1, 2, -1> wyz;
		swizzle<3, 1, 3, -1> wyw;
		swizzle<3, 2, 0, -1> wzx;
		swizzle<3, 2, 1, -1> wzy;
		swizzle<3, 2, 2, -1> wzz;
		swizzle<3, 2, 3, -1> wzw;
		swizzle<3, 3, 0, -1> wwx;
		swizzle<3, 3, 1, -1> wwy;
		swizzle<3, 3, 2, -1> wwz;
		swizzle<3, 3, 3, -1> www;

		//float4 variations
		swizzle<0, 0, 0, 0> xxxx;
		swizzle<0, 0, 0, 1> xxxy;
		swizzle<0, 0, 0, 2> xxxz;
		swizzle<0, 0, 0, 3> xxxw;
		swizzle<0, 0, 1, 0> xxyx;
		swizzle<0, 0, 1, 1> xxyy;
		swizzle<0, 0, 1, 2> xxyz;
		swizzle<0, 0, 1, 3> xxyw;
		swizzle<0, 0, 2, 0> xxzx;
		swizzle<0, 0, 2, 1> xxzy;
		swizzle<0, 0, 2, 2> xxzz;
		swizzle<0, 0, 2, 3> xxzw;
		swizzle<0, 0, 3, 0> xxwx;
		swizzle<0, 0, 3, 1> xxwy;
		swizzle<0, 0, 3, 2> xxwz;
		swizzle<0, 0, 3, 3> xxww;
		swizzle<0, 1, 0, 0> xyxx;
		swizzle<0, 1, 0, 1> xyxy;
		swizzle<0, 1, 0, 2> xyxz;
		swizzle<0, 1, 0, 3> xyxw;
		swizzle<0, 1, 1, 0> xyyx;
		swizzle<0, 1, 1, 1> xyyy;
		swizzle<0, 1, 1, 2> xyyz;
		swizzle<0, 1, 1, 3> xyyw;
		swizzle<0, 1, 2, 0> xyzx;
		swizzle<0, 1, 2, 1> xyzy;
		swizzle<0, 1, 2, 2> xyzz;
		swizzle<0, 1, 2, 3> xyzw;
		swizzle<0, 1, 3, 0> xywx;
		swizzle<0, 1, 3, 1> xywy;
		swizzle<0, 1, 3, 2> xywz;
		swizzle<0, 1, 3, 3> xyww;
		swizzle<0, 2, 0, 0> xzxx;
		swizzle<0, 2, 0, 1> xzxy;
		swizzle<0, 2, 0, 2> xzxz;
		swizzle<0, 2, 0, 3> xzxw;
		swizzle<0, 2, 1, 0> xzyx;
		swizzle<0, 2, 1, 1> xzyy;
		swizzle<0, 2, 1, 2> xzyz;
		swizzle<0, 2, 1, 3> xzyw;
		swizzle<0, 2, 2, 0> xzzx;
		swizzle<0, 2, 2, 1> xzzy;
		swizzle<0, 2, 2, 2> xzzz;
		swizzle<0, 2, 2, 3> xzzw;
		swizzle<0, 2, 3, 0> xzwx;
		swizzle<0, 2, 3, 1> xzwy;
		swizzle<0, 2, 3, 2> xzwz;
		swizzle<0, 2, 3, 3> xzww;
		swizzle<0, 3, 0, 0> xwxx;
		swizzle<0, 3, 0, 1> xwxy;
		swizzle<0, 3, 0, 2> xwxz;
		swizzle<0, 3, 0, 3> xwxw;
		swizzle<0, 3, 1, 0> xwyx;
		swizzle<0, 3, 1, 1> xwyy;
		swizzle<0, 3, 1, 2> xwyz;
		swizzle<0, 3, 1, 3> xwyw;
		swizzle<0, 3, 2, 0> xwzx;
		swizzle<0, 3, 2, 1> xwzy;
		swizzle<0, 3, 2, 2> xwzz;
		swizzle<0, 3, 2, 3> xwzw;
		swizzle<0, 3, 3, 0> xwwx;
		swizzle<0, 3, 3, 1> xwwy;
		swizzle<0, 3, 3, 2> xwwz;
		swizzle<0, 3, 3, 3> xwww;
		swizzle<1, 0, 0, 0> yxxx;
		swizzle<1, 0, 0, 1> yxxy;
		swizzle<1, 0, 0, 2> yxxz;
		swizzle<1, 0, 0, 3> yxxw;
		swizzle<1, 0, 1, 0> yxyx;
		swizzle<1, 0, 1, 1> yxyy;
		swizzle<1, 0, 1, 2> yxyz;
		swizzle<1, 0, 1, 3> yxyw;
		swizzle<1, 0, 2, 0> yxzx;
		swizzle<1, 0, 2, 1> yxzy;
		swizzle<1, 0, 2, 2> yxzz;
		swizzle<1, 0, 2, 3> yxzw;
		swizzle<1, 0, 3, 0> yxwx;
		swizzle<1, 0, 3, 1> yxwy;
		swizzle<1, 0, 3, 2> yxwz;
		swizzle<1, 0, 3, 3> yxww;
		swizzle<1, 1, 0, 0> yyxx;
		swizzle<1, 1, 0, 1> yyxy;
		swizzle<1, 1, 0, 2> yyxz;
		swizzle<1, 1, 0, 3> yyxw;
		swizzle<1, 1, 1, 0> yyyx;
		swizzle<1, 1, 1, 1> yyyy;
		swizzle<1, 1, 1, 2> yyyz;
		swizzle<1, 1, 1, 3> yyyw;
		swizzle<1, 1, 2, 0> yyzx;
		swizzle<1, 1, 2, 1> yyzy;
		swizzle<1, 1, 2, 2> yyzz;
		swizzle<1, 1, 2, 3> yyzw;
		swizzle<1, 1, 3, 0> yywx;
		swizzle<1, 1, 3, 1> yywy;
		swizzle<1, 1, 3, 2> yywz;
		swizzle<1, 1, 3, 3> yyww;
		swizzle<1, 2, 0, 0> yzxx;
		swizzle<1, 2, 0, 1> yzxy;
		swizzle<1, 2, 0, 2> yzxz;
		swizzle<1, 2, 0, 3> yzxw;
		swizzle<1, 2, 1, 0> yzyx;
		swizzle<1, 2, 1, 1> yzyy;
		swizzle<1, 2, 1, 2> yzyz;
		swizzle<1, 2, 1, 3> yzyw;
		swizzle<1, 2, 2, 0> yzzx;
		swizzle<1, 2, 2, 1> yzzy;
		swizzle<1, 2, 2, 2> yzzz;
		swizzle<1, 2, 2, 3> yzzw;
		swizzle<1, 2, 3, 0> yzwx;
		swizzle<1, 2, 3, 1> yzwy;
		swizzle<1, 2, 3, 2> yzwz;
		swizzle<1, 2, 3, 3> yzww;
		swizzle<1, 3, 0, 0> ywxx;
		swizzle<1, 3, 0, 1> ywxy;
		swizzle<1, 3, 0, 2> ywxz;
		swizzle<1, 3, 0, 3> ywxw;
		swizzle<1, 3, 1, 0> ywyx;
		swizzle<1, 3, 1, 1> ywyy;
		swizzle<1, 3, 1, 2> ywyz;
		swizzle<1, 3, 1, 3> ywyw;
		swizzle<1, 3, 2, 0> ywzx;
		swizzle<1, 3, 2, 1> ywzy;
		swizzle<1, 3, 2, 2> ywzz;
		swizzle<1, 3, 2, 3> ywzw;
		swizzle<1, 3, 3, 0> ywwx;
		swizzle<1, 3, 3, 1> ywwy;
		swizzle<1, 3, 3, 2> ywwz;
		swizzle<1, 3, 3, 3> ywww;
		swizzle<2, 0, 0, 0> zxxx;
		swizzle<2, 0, 0, 1> zxxy;
		swizzle<2, 0, 0, 2> zxxz;
		swizzle<2, 0, 0, 3> zxxw;
		swizzle<2, 0, 1, 0> zxyx;
		swizzle<2, 0, 1, 1> zxyy;
		swizzle<2, 0, 1, 2> zxyz;
		swizzle<2, 0, 1, 3> zxyw;
		swizzle<2, 0, 2, 0> zxzx;
		swizzle<2, 0, 2, 1> zxzy;
		swizzle<2, 0, 2, 2> zxzz;
		swizzle<2, 0, 2, 3> zxzw;
		swizzle<2, 0, 3, 0> zxwx;
		swizzle<2, 0, 3, 1> zxwy;
		swizzle<2, 0, 3, 2> zxwz;
		swizzle<2, 0, 3, 3> zxww;
		swizzle<2, 1, 0, 0> zyxx;
		swizzle<2, 1, 0, 1> zyxy;
		swizzle<2, 1, 0, 2> zyxz;
		swizzle<2, 1, 0, 3> zyxw;
		swizzle<2, 1, 1, 0> zyyx;
		swizzle<2, 1, 1, 1> zyyy;
		swizzle<2, 1, 1, 2> zyyz;
		swizzle<2, 1, 1, 3> zyyw;
		swizzle<2, 1, 2, 0> zyzx;
		swizzle<2, 1, 2, 1> zyzy;
		swizzle<2, 1, 2, 2> zyzz;
		swizzle<2, 1, 2, 3> zyzw;
		swizzle<2, 1, 3, 0> zywx;
		swizzle<2, 1, 3, 1> zywy;
		swizzle<2, 1, 3, 2> zywz;
		swizzle<2, 1, 3, 3> zyww;
		swizzle<2, 2, 0, 0> zzxx;
		swizzle<2, 2, 0, 1> zzxy;
		swizzle<2, 2, 0, 2> zzxz;
		swizzle<2, 2, 0, 3> zzxw;
		swizzle<2, 2, 1, 0> zzyx;
		swizzle<2, 2, 1, 1> zzyy;
		swizzle<2, 2, 1, 2> zzyz;
		swizzle<2, 2, 1, 3> zzyw;
		swizzle<2, 2, 2, 0> zzzx;
		swizzle<2, 2, 2, 1> zzzy;
		swizzle<2, 2, 2, 2> zzzz;
		swizzle<2, 2, 2, 3> zzzw;
		swizzle<2, 2, 3, 0> zzwx;
		swizzle<2, 2, 3, 1> zzwy;
		swizzle<2, 2, 3, 2> zzwz;
		swizzle<2, 2, 3, 3> zzww;
		swizzle<2, 3, 0, 0> zwxx;
		swizzle<2, 3, 0, 1> zwxy;
		swizzle<2, 3, 0, 2> zwxz;
		swizzle<2, 3, 0, 3> zwxw;
		swizzle<2, 3, 1, 0> zwyx;
		swizzle<2, 3, 1, 1> zwyy;
		swizzle<2, 3, 1, 2> zwyz;
		swizzle<2, 3, 1, 3> zwyw;
		swizzle<2, 3, 2, 0> zwzx;
		swizzle<2, 3, 2, 1> zwzy;
		swizzle<2, 3, 2, 2> zwzz;
		swizzle<2, 3, 2, 3> zwzw;
		swizzle<2, 3, 3, 0> zwwx;
		swizzle<2, 3, 3, 1> zwwy;
		swizzle<2, 3, 3, 2> zwwz;
		swizzle<2, 3, 3, 3> zwww;
		swizzle<3, 0, 0, 0> wxxx;
		swizzle<3, 0, 0, 1> wxxy;
		swizzle<3, 0, 0, 2> wxxz;
		swizzle<3, 0, 0, 3> wxxw;
		swizzle<3, 0, 1, 0> wxyx;
		swizzle<3, 0, 1, 1> wxyy;
		swizzle<3, 0, 1, 2> wxyz;
		swizzle<3, 0, 1, 3> wxyw;
		swizzle<3, 0, 2, 0> wxzx;
		swizzle<3, 0, 2, 1> wxzy;
		swizzle<3, 0, 2, 2> wxzz;
		swizzle<3, 0, 2, 3> wxzw;
		swizzle<3, 0, 3, 0> wxwx;
		swizzle<3, 0, 3, 1> wxwy;
		swizzle<3, 0, 3, 2> wxwz;
		swizzle<3, 0, 3, 3> wxww;
		swizzle<3, 1, 0, 0> wyxx;
		swizzle<3, 1, 0, 1> wyxy;
		swizzle<3, 1, 0, 2> wyxz;
		swizzle<3, 1, 0, 3> wyxw;
		swizzle<3, 1, 1, 0> wyyx;
		swizzle<3, 1, 1, 1> wyyy;
		swizzle<3, 1, 1, 2> wyyz;
		swizzle<3, 1, 1, 3> wyyw;
		swizzle<3, 1, 2, 0> wyzx;
		swizzle<3, 1, 2, 1> wyzy;
		swizzle<3, 1, 2, 2> wyzz;
		swizzle<3, 1, 2, 3> wyzw;
		swizzle<3, 1, 3, 0> wywx;
		swizzle<3, 1, 3, 1> wywy;
		swizzle<3, 1, 3, 2> wywz;
		swizzle<3, 1, 3, 3> wyww;
		swizzle<3, 2, 0, 0> wzxx;
		swizzle<3, 2, 0, 1> wzxy;
		swizzle<3, 2, 0, 2> wzxz;
		swizzle<3, 2, 0, 3> wzxw;
		swizzle<3, 2, 1, 0> wzyx;
		swizzle<3, 2, 1, 1> wzyy;
		swizzle<3, 2, 1, 2> wzyz;
		swizzle<3, 2, 1, 3> wzyw;
		swizzle<3, 2, 2, 0> wzzx;
		swizzle<3, 2, 2, 1> wzzy;
		swizzle<3, 2, 2, 2> wzzz;
		swizzle<3, 2, 2, 3> wzzw;
		swizzle<3, 2, 3, 0> wzwx;
		swizzle<3, 2, 3, 1> wzwy;
		swizzle<3, 2, 3, 2> wzwz;
		swizzle<3, 2, 3, 3> wzww;
		swizzle<3, 3, 0, 0> wwxx;
		swizzle<3, 3, 0, 1> wwxy;
		swizzle<3, 3, 0, 2> wwxz;
		swizzle<3, 3, 0, 3> wwxw;
		swizzle<3, 3, 1, 0> wwyx;
		swizzle<3, 3, 1, 1> wwyy;
		swizzle<3, 3, 1, 2> wwyz;
		swizzle<3, 3, 1, 3> wwyw;
		swizzle<3, 3, 2, 0> wwzx;
		swizzle<3, 3, 2, 1> wwzy;
		swizzle<3, 3, 2, 2> wwzz;
		swizzle<3, 3, 2, 3> wwzw;
		swizzle<3, 3, 3, 0> wwwx;
		swizzle<3, 3, 3, 1> wwwy;
		swizzle<3, 3, 3, 2> wwwz;
		swizzle<3, 3, 3, 3> wwww;


		float v[4];
	};

	//Default constructor
	float4(void);

	//Copy constructor
	float4(const float4& rhs);
	float4(const float2& rhs, float zz, float ww);
	float4(float xx, const float2& rhs, float ww);
	float4(float xx, float yy, const float2& rhs);
	float4(const float3& rhs, float ww);
	float4(float xx, const float3& rhs);

	//Non-default constructor
	float4(float _x, float _y, float _z, float _w);

	//Operator overloads
	float4& operator=(const float4& rhs); //Assignment
	float4& operator=(const float rhs);
	float4 operator-(void) const; //Unary negation operator

	float4 operator+(const float4& rhs) const;
	float4 operator-(const float4& rhs) const;
	float4 operator*(const float rhs) const;
	float4 operator/(const float rhs) const;

	float4& operator+=(const float4& rhs);
	float4& operator-=(const float4& rhs);
	float4& operator*=(const float rhs);
	float4& operator/=(const float rhs);

	bool operator==(const float4& rhs) const;
	bool operator!=(const float4& rhs) const;

  float &operator[](const unsigned idx);
  float operator[](const unsigned idx) const;

  operator float*(void)
  {
    return v;
  }

	//Vector operations
	float Dot(const float4& rhs) const;
	float4 Cross(const float4& rhs) const;
  float4 Project(const float4& rhs) const;
	float Length(void) const;
	float LengthSq(void) const;
	void Normalize(float *pLen = nullptr);
  float4 Normal(float *pLen = nullptr) const;
	void Zero(void);
	void Print(void) const;
};
