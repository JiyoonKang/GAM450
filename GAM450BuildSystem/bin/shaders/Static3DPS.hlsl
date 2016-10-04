#include "ShaderIncludes.h"

Texture2D objTexture : register(t0);

SamplerState Sampler : register(TRILINEAR_WRAP);

static const float PI = 3.14159265f;

cbuffer Constants : register(b2)
{
	float3 EyePosition;
	float  isSkydome;
	float4 objectColor;
}

struct PixelInputType
{
    float4 worldPos     : WORLDPOS;
    float4 position     : SV_POSITION;
    float3 normal       : NORMAL;
    float2 uv           : UV;
    float3 tangent      : TANGENT;
    float3 bitangent    : BITANGENT;
    float4 color : COLOR;
};

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 main(PixelInputType input) : SV_Target
{
	return objectColor;
}