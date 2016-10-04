
cbuffer DefaultBuffer
{
  float4x4 transform;
}

Texture2D gTexture : register(t0);

SamplerState gSampler : register(s0);


struct PSInput
{
  float4 color : COLOR;
  float2 texCoord : TEXCOORD0;    // texture coordinates
};

float4 main(PSInput input) : SV_TARGET
{
  return input.color * gTexture.Sample(gSampler, input.texCoord);
}
