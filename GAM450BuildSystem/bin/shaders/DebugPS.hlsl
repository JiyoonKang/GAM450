#include "ShaderIncludes.h"

Texture2D target  : register(t0);

SamplerState Sampler : register(BILINEAR_WRAP);
static const float PI = 3.14159265f; 

cbuffer Constants : register(b2)
{
	float3 EyePosition;
	float  isSkydome;
	float4 emissiveColor;
	float offset;
}

struct PixelInputType
{
    float4 position     : SV_POSITION;
    float2 uv           : UV;
};

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 main(PixelInputType input) : SV_Target
{
	return target.Sample(Sampler, input.uv).xyzw;
}