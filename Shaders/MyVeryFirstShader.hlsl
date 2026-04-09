struct ConstantData
{
    float4x4 World;
	float4 color;
	
	// float4 materialDiffuse;
    float4 materialSpecular;
	float3 materialAmbient;
	float materialAlpha;
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
    float4x4 gInvView;
};

cbuffer LightPass : register(b2)
{
	float3 lightDirection;
	float lightIntencity;
};

struct VS_IN
{
	float3 posL : POSITION0;
	float4 col : COLOR0;
 	float2 tex : TEXCOORD0;
 	float3 normal : NORMAL;
};

struct PS_IN
{
	float4 posH : SV_POSITION;
 	float4 col : COLOR;
 	float2 tex : TEXCOORD0;
 	float3 normal : NORMAL;
	float3 worldPos : POSITION;
};

Texture2D DiffuseMap : register(t0);
SamplerState Sampler : register(s0);

PS_IN VSMain( VS_IN input )
{
	PS_IN output = (PS_IN)0;
	
    float4 PosW = mul(float4(input.posL.xyz, 1.0f), constantData.World);
    float4 posV = mul(PosW, gView);
	output.posH = mul(posV, gProjection);

    output.col = input.col;
    output.tex = input.tex;
	output.worldPos = PosW.xyz;
	output.normal = mul(float4(input.normal, 0.0f), constantData.World).xyz;
    output.normal = normalize(output.normal);
	return output;
}

float4 PSMain( PS_IN input ) : SV_Target
{
	float4 materialDiffuse = DiffuseMap.Sample(Sampler, input.tex.xy);
	
	
	float3 lightDir = normalize(lightDirection); 
	float3 viewDir = normalize(gInvView[3].xyz - input.worldPos);
	float3 reflectDir = reflect(-lightDir, input.normal); 
	float RdotV = max(dot(reflectDir, viewDir), 0.0);
	
	float4 diffuse = materialDiffuse * lightIntencity * dot(lightDir, input.normal);
	float4 specular = constantData.materialSpecular * lightIntencity * pow(RdotV, constantData.materialAlpha);
	float ambientIntencity = 0.618f;
	float4 ambient = float4(constantData.materialAmbient.xyz, 1.0) * materialDiffuse * ambientIntencity;
	return diffuse + specular + ambient;
}