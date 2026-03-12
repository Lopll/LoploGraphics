struct ConstantData
{
	float4x4 transform;
	float4 color;
};

cbuffer ConstantBuffer : register(b0)
{
	ConstantData constantData;
}

struct VS_IN
{
	float4 pos : POSITION0;
	float4 col : COLOR0;
};

struct PS_IN
{
	float4 pos : SV_POSITION;
 	float4 col : COLOR;
};

PS_IN VSMain( VS_IN input )
{
	PS_IN output = (PS_IN)0;
	
	// output.pos = float4(input.pos.xyz + constantData.offset.xyz, 1.0f);
	output.pos = mul(float4(input.pos.xyz, 1.0f), constantData.transform);
	output.col = constantData.color;
	
	return output;
}

float4 PSMain( PS_IN input ) : SV_Target
{
	float4 col = input.col;
	return col;
}