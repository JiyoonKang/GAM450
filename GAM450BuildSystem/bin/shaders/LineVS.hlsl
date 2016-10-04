cbuffer Matrix3D : register(b0)
{
	float4x4 view;
    float4x4 viewInverse;
	float4x4 projection;
    float4x4 projectionInverse;
    float4x4 previousViewProjection;
};


struct LineVertex
{
    float4 position     : POSITION;
    float4 color		: COLOR;
};

struct PixelInputType
{
    float4 position     : SV_POSITION;
    float4 color		: COLOR;
};


PixelInputType main(LineVertex input)
{
    PixelInputType output;
    
	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(float4(input.position.xyz, 1.0f), transpose(view));
    output.position = mul(output.position, transpose(projection));
	
	output.color = input.color;
    
    return output;
}