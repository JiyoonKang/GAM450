/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: particleSystemPS.hlsl
 * Author: Esteban Maldonado
 * Class: GAM300/GAM302/GAM352/GAM375/GAM400
 ******************************************************************************/

/////////////
// GLOBALS //
/////////////
SamplerState Sampler : register(s0);

Texture2D objTexture : register(t0);

//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
  float4 position : SV_POSITION;
  float2 uv : TEXTURE;
  float4 color : COLOR;
};

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 main(PixelInputType input) : SV_TARGET
{
  int width, height;
  float4 texColor = { 1, 1, 1, 1 };
  objTexture.GetDimensions(width, height);
  
  //Determine if we have a valid texture to fetch texels from
  if (width > 0 && height > 0)
    texColor = objTexture.Sample(Sampler, input.uv);
  
  return input.color * texColor;
}
