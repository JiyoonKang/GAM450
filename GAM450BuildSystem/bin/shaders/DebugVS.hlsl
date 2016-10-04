
cbuffer Object : register(b1)
{
	float4x4 worldMatrix;
}

struct VertexInputType
{
    float3 position     : POSITION;
    float3 normal       : NORMAL;
    float3 tangent      : TANGENT;
    float3 bitangent    : BITANGENT;
    float2 uv           : TEXTURE;
    float4 color		: COLOR;
};

struct PixelInputType
{
    float4 position     : SV_POSITION;
    float2 uv           : UV;
};


PixelInputType main(VertexInputType input)
{
    PixelInputType output;
    
    output.position = mul(float4(input.position, 1), (worldMatrix));

	output.uv = float2((input.position.xy) * 0.5 + (0.5).xx);

	//Invert y coordinate
	output.uv.y = 1 - output.uv.y;
    return output;
}