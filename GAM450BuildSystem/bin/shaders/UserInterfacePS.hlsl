#include "ShaderIncludes.h"

Texture2D awesomiumTex : register(t0);


SamplerState Sampler : register(BILINEAR_WRAP);
static const float PI = 3.14159265f; 

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
	return float4(awesomiumTex.Sample(Sampler, input.uv).rgba);
	
}