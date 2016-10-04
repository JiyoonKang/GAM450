cbuffer Matrix3D : register(b0)
{
	float4x4 view;
	float4x4 viewInverse;
	float4x4 projection;
	float4x4 projectionInverse;
	float4x4 previousViewProjection;
};

struct PointLightData
{
    float4 lightColor     : COLOR;
    float4 position       : POSITION;
	float4 center :	 CENTER;
    float2 rangeAttenuation      : RANGE;
    float2 pad    : PAD;
	
};

struct LightDataOut
{
    float4 position     : SV_POSITION;
	float4 lightColor : COLOR;
	float3 posW : POSITION;
	float2 rangeAttenuation : RANGE;

};


LightDataOut main(PointLightData input)
{
    LightDataOut output;
    
	output.posW = input.position.xyz;
	output.lightColor = input.lightColor;
	output.rangeAttenuation = input.rangeAttenuation;

	output.position = mul(float4(input.position.xyz, 1), transpose(view));
	output.position = mul(output.position, transpose(projection));


    return output;
}