#include "ShaderIncludes.h"

Texture2D positionTex : register(t0);
Texture2D normalTex   : register(t1);
Texture2D diffuseTex  : register(t2);
Texture2D skyTex      : register(t3);
Texture2D debugTex    : register(t4);

SamplerState Sampler : register(BILINEAR_WRAP);
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

struct LightDataOut
{
	float4 position     : SV_POSITION;
	float4 lightColor : COLOR;
	float3 posW : POSITION;
	float2 rangeAttenuation : RANGE;

};

void GetBufferInformation(in float2 texturePosition, out float3 normal, out float3 position, out float shininess)
{
	int3 i = int3(texturePosition.xy, 0);
	float4 sample = normalTex.Load(i);
	position = positionTex.Load(i).xyz;
	normal = normalTex.Load(i);
	shininess = 18;

}

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 main(LightDataOut input) : SV_Target
{
	float3 normal, position;
	float shininess;

	//GetBufferInformation(input.position.xy, normal, position, shininess);

	int3 i = int3(input.position.xy, 0);
	float4 sample = normalTex.Load(i);
	position = positionTex.Load(i).xyz;
	normal = normalTex.Load(i);
	shininess = 0.3;

	float3 L = input.posW - position;
	float distance = length(L);

	if (distance > input.rangeAttenuation.x)
	{
		discard;
	}

	float attenuation = 1.0;

	if (distance > input.rangeAttenuation.y)
	{
		attenuation = saturate(1 - (distance - input.rangeAttenuation.y) / (input.rangeAttenuation.x - input.rangeAttenuation.y));
	}

	float3 diffuse = input.lightColor.xyz / PI;

	L = normalize(L);

	float3 E = normalize(EyePosition - position);
	float3 H = normalize(L + E);

	float3 Ks = float3(0.01, 0.01, 0.01);

	float specularTerm = shininess;

	float3 D = ((specularTerm + 2) / (2 * PI)) * (pow(max(0.01, dot(normal, H)), specularTerm));
	float3 F = Ks + ((1 - Ks) * (pow(1 - (max(0.0, dot(L, H))), 5)));
	float3 G = 1 / pow(max(0.0001, dot(L, H)), 2);
	float3 BRDF = (F * G * D) / 4;
	
	float3 lightOut = (diffuse + BRDF) * (max(0.0, dot(normal, L)) * (input.lightColor.rgb * PI));
	
	return float4(lightOut.xyz * attenuation, 1.0);//  -float4(attenuation, attenuation, attenuation, attenuation);
}