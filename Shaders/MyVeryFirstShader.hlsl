struct ConstantData
{
    float4x4 World;
	float4 color;
};

cbuffer ConstantBuffer : register(b0)
{
	ConstantData constantData;
}

cbuffer MainPass : register(b1)
{
    float4x4 gProjection;
    float4x4 gInvProjection;
    float4x4 gView;
};

struct VS_IN
{
	float3 posL : POSITION0;
	float4 col : COLOR0;
};

struct PS_IN
{
	float4 posH : SV_POSITION;
 	float4 col : COLOR;
};

// Texture2D DiffuseMap : register(t0);
// SamplerState Sampler ; register(s0);

PS_IN VSMain( VS_IN input )
{
	PS_IN output = (PS_IN)0;
	
    float4 PosW = mul(float4(input.posL.xyz, 1.0f), constantData.World);
    float4 posV = mul(PosW, gView);
	output.posH = mul(posV, gProjection);



    output.col = input.col;
	
	return output;
}

float4 PSMain( PS_IN input ) : SV_Target
{
	float4 col = constantData.color;//DiffuseMap.Sample(Sampler, input.tex.xy);//
	return col;
}