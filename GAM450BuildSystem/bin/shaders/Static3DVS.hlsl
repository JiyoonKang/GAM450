cbuffer Matrix3D : register(b0)
{
	float4x4 view;
    float4x4 viewInverse;
	float4x4 projection;
    float4x4 projectionInverse;
    float4x4 previousViewProjection;
};

cbuffer Object : register(b1)
{
    float4x4 worldMatrix;
}

cbuffer Constants : register(b2)
{
	float3 EyePosition;
	float  isSkydome;
	float4 objectColor;
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
    float4 worldPos     : WORLDPOS;
    float4 position     : SV_POSITION;
    float3 normal       : NORMAL;
    float2 uv           : UV;
    float3 tangent      : TANGENT;
    float3 bitangent    : BITANGENT;
    float4 color		: COLOR;
};


PixelInputType main(VertexInputType input)
{
    PixelInputType output;
    
	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(float4(input.position, 1), (worldMatrix));
    output.position = mul(output.position, transpose(view));
    output.position = mul(output.position, transpose(projection));

	//output.position = float4(input.position, 1);
    output.worldPos = mul(float4(input.position, 1), worldMatrix);
	
	//output.worldPos = mul(float4(input.position, 1), transpose(worldMatrix));
	output.uv = input.uv / 2;
	
    output.tangent = input.tangent;
    output.bitangent = input.bitangent;
	
    output.normal = mul(input.normal, ((float3x3)worldMatrix));
    output.normal = normalize(output.normal);
	
	output.color = input.color;
    
    return output;
}