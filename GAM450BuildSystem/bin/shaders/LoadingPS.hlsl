#include "ShaderIncludes.h"

Texture2D loadingTex  : register(t0);

SamplerState Sampler : register(BILINEAR_WRAP);


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
	return loadingTex.Sample(Sampler, input.uv).xyzw;
}