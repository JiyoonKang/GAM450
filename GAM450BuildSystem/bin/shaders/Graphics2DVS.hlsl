cbuffer Matrix2D : register(b3)
{
	float4x4 projection;
	float2 screenSize;
};

struct VertexInputType
{
	float3 position     : POSITION;
	float4 color		: COLOR;
    float2 uv           : UV;
};

struct PixelInputType
{

	float4 color		: COLOR;
	float4 position     : SV_POSITION;
	float2 uv           : UV;
};


PixelInputType main(VertexInputType input)
{
    PixelInputType output;
    
	output.position = (float4(input.position.xyz, 1));// , (projection));
	
	
	//output.uv = float2((input.position.xy) * 0.5 + (0.5).xx);
	output.color = input.color;
	output.uv = input.uv;
    
    return output;
}