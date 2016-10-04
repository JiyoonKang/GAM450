#include "ShaderIncludes.h"

Texture2D objTexture : register(t0);
Texture2D objNormal  : register(t1);
Texture2D objEmisive : register(t2);

SamplerState Sampler : register(TRILINEAR_WRAP);

static const float PI = 3.14159265f;
struct OutputTextures
{
	float4 Position : COLOR0;
	float4 Normal   : COLOR1;
	float4 Diffused : COLOR2;
	float4 Specular : COLOR3;
	float4 Emitted  : COLOR4;
	float4 Skydome  : COLOR5;
};

cbuffer Constants : register(b2)
{
	float3 EyePosition;
	float  isSkydome;
	float4 emissiveColor;
	float offset;
	float4 apexColor;
	float4 centerColor;
}

struct PixelInputType
{
    float4 worldPos     : WORLDPOS;
    float4 position     : SV_POSITION;
    float3 normal       : NORMAL;
    float2 uv           : UV;
    float3 tangent      : TANGENT;
    float3 bitangent    : BITANGENT;
    float4 color        : COLOR;
	float4 domePosition : TEXCOORD0;
};

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
OutputTextures main(PixelInputType input) : SV_Target
{
	OutputTextures deferred;

	deferred.Position = input.worldPos;
	deferred.Normal = float4(input.normal, 1);

	float width, height;
	objNormal.GetDimensions(width, height);

	if (width > 0 && height > 0)
	{
		float2 tex = input.uv * 2.0;
			
		float3 normal = normalize(input.normal);
		
		float3 abc = (objNormal.Sample(Sampler, tex).xyz * 2.0) - float3(1, 1, 1);
		float3 newNorm = (input.tangent * abc.x) + (abc.y * cross(normal, input.tangent)) + (abc.z * normal);

		deferred.Normal = float4(newNorm, 1);
	}

	deferred.Diffused =  objTexture.Sample(Sampler, input.uv * 2);
	deferred.Emitted = objEmisive.Sample(Sampler, input.uv * 2) * float4(emissiveColor.xyz, 1);

	if (deferred.Emitted.w == 0.0)
		deferred.Emitted = float4(0.0, 0.0, 0.0, 0.0);

	deferred.Specular = float4(0.5, 0.5, 0.5, 1);
	deferred.Skydome = float4(0, 0, 0, 0);
	
		
    if (isSkydome == 1.0f)
	{
		deferred.Skydome = float4(1, 1, 1, 1);
		float height ;
		height = input.domePosition.y;
		if(height < 0.0)
		{
			height = 0.0f;
		}

		float3 E = normalize(input.worldPos.xyz - EyePosition);
		float u = (-atan2(E.z, E.x)) / (2 * PI);
		float v = acos(E.y) / (PI);
		input.uv.x = u;
		input.uv.y = v;

		//deferred.Diffused = objTexture.Sample(Sampler, input.uv);
		//deferred.Diffused = centerColor;
		deferred.Diffused = lerp(centerColor, apexColor, height);
	}
	
	return deferred;
}