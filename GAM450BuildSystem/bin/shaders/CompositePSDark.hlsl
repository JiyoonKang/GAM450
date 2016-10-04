#include "ShaderIncludes.h"

Texture2D positionTex  : register(t0);
Texture2D normalTex    : register(t1);
Texture2D diffuseTex   : register(t2);
Texture2D skyTex       : register(t3);
Texture2D emittedTex   : register(t4);
Texture2D debugTex     : register(t5);
Texture2D lightTex     : register(t6);
Texture2D particleTex  : register(t7);

SamplerState Sampler : register(ANISOTROPIC_WRAP);
static const float PI = 3.14159265f; 

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
    float4 position     : SV_POSITION;
    float2 uv           : UV;
};

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 main(PixelInputType input) : SV_Target
{

	float4 diffused = diffuseTex.Sample(Sampler, input.uv);
	float4 emitted = emittedTex.Sample(Sampler, input.uv);
	float sky = skyTex.Sample(Sampler, input.uv);

	//If the current pixel curresponds to the skydome, just return diffused
	if (sky)
		return diffused;

	//Since the pixel is not skydome, do appropriate lighting calculations

	//World space position
	float3 position = positionTex.Sample(Sampler, input.uv);

	//World space normals
	float3 N = normalTex.Sample(Sampler, input.uv);

	//Eye or view vector
	float3 E = normalize(EyePosition.xyz - position);

	//Light direction vector
	float3 L = normalize(float3(50, 50, 150) - position);

	//Halfway vector
	float3 H = normalize(L + E);

	//Specular color
	float3 Ks = float3(1.0, 1.0, 1.0);

	//Ambient light
	float3 light = float3(0.2, 0.2, 0.2) * diffused.xyz /PI;

	//First element of BRDF calculation (diffused element)
	float3 BRDF = diffused / PI;

	//Material roughness
	float specularTerm = 18;

	//BRDF Calculations
	float3 D = ((specularTerm + 2) / (2 * PI)) * (pow(max(0.0, dot(N, H)), specularTerm));
	float3 F = Ks + (1 - Ks) * (pow(max(0.0, (1 - dot(L, H))), 5));
	float3 G = 1 / pow(max(0.0001, dot(L, H)), 2);
	BRDF += (D * F * G) / 4;

	light += ((BRDF) * (max(0.0, dot(N, L)) * (PI * float4(1.0, 1.0, 1.0, 1))));

	float4 debugDraw = debugTex.Sample(Sampler, input.uv);

		float4 pointLights = lightTex.Sample(Sampler, input.uv).xyzw;

		float4 particles = particleTex.Sample(Sampler, input.uv).xyzw;

    // Editor
	return debugDraw + pointLights + emitted + particles;
    // Game
	//return float4(light.xyz, 1) + debugDraw + pointLights;// +awesomium;
	
}