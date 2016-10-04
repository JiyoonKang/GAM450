cbuffer Matrix3D : register(b0)
{
	float4x4 view;
	float4x4 viewInverse;
	float4x4 projection;
	float4x4 projectionInverse;
	float4x4 previousViewProjection;
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
struct LightDataOut
{
	float4 position     : SV_POSITION;
	float4 lightColor : COLOR;
	float3 posW : POSITION;
	float2 rangeAttenuation : RANGE;

};

//Quad struct for the light geometry shader
static const float2 g_LightQuadPosition[4] =
{
	float2(-0.5, 0.5),
	float2(0.5, 0.5),
	float2(-0.5, -0.5),
	float2(0.5, -0.5)
};

[maxvertexcount(4)]
void main(point LightDataOut p[1], inout TriangleStream<LightDataOut> SpriteStream)
{
	LightDataOut L = p[0];
	float2 Range = L.rangeAttenuation.xx;
	float3 eyeToLight = L.posW - EyePosition;
	
	float dist = length(eyeToLight);
	float area = L.rangeAttenuation.x;
	
	if(dist < area)
	{
		for (int i = 0; i < 4; ++i)
		{
			L.position = float4(g_LightQuadPosition[i], 0, 0);
			SpriteStream.Append(L);
		}
	}
	
	else
	{
		for (int i = 0; i < 4; ++i)
		{
			L.position = p[0].position + float4(g_LightQuadPosition[i] * Range * 10.5, 0, 0);
			SpriteStream.Append(L);
		}
	}
	

	SpriteStream.RestartStrip();
}