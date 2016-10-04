#include "ShaderIncludes.h"

Texture2D objectTexture : register(t0);
Texture2D objectPaintTexture : register(t1);
SamplerState Sampler : register(POINT_CLAMP);

struct PixelInputType
{

	float4 color		: COLOR;
    float4 position     : SV_POSITION;
    float2 uv           : UV;
};

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 main(PixelInputType input) : SV_Target
{
	return input.color;// *objectTexture.Sample(Sampler, input.uv);
}