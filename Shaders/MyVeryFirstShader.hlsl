struct PointLight
{
	float intencity;
	float3 position;
	float3 color;
	float radius;
};

struct DirectionalLight
{
	float intencity;
	float3 direction;
};

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
};

cbuffer MainPass : register(b1)
{
    float4x4 gProjection;
    float4x4 gInvProjection;
    float4x4 gView;
    float4x4 gInvView;
};

cbuffer LightPass : register(b2)
{
	DirectionalLight directionalLight;
	float4x4 shadowView;
	PointLight pointLight[100];
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

Texture2D ShadowMap : register(t1);
SamplerComparisonState  ShadowSampler : register(s1);

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

float4 CalcLight(float3 viewDir, float3 normal, float3 worldPos, float4 materialDiffuse, float alpha, float3 lightDirection, float lightIntencity, float3 color, bool castShadows)
{
	float3 lightDir = normalize(-lightDirection);
	float3 reflectDir = reflect(normalize(lightDirection), normal); 
	float RdotV = max(dot(reflectDir, viewDir), 0.0);
	
	float4 diffuse;
	if(constantData.materialAlpha != -1)
	{
        float NdotL = max(0.2f, dot(lightDir, normal));
        diffuse = materialDiffuse * lightIntencity * NdotL;
    }
	else
	{
		diffuse = constantData.color * lightIntencity;
	}	
	
	float4 specular = constantData.materialSpecular * lightIntencity * pow(RdotV, alpha);
	float ambientIntencity = 0.618f * lightIntencity;
	float4 ambient = float4(constantData.materialAmbient.xyz, 1.0) * materialDiffuse * ambientIntencity;
	
	if(castShadows)
	{
		float4 lightSpacePos = mul(float4(worldPos, 1.0f), shadowView);
		lightSpacePos.xyz /= lightSpacePos.w;
		float2 uv = lightSpacePos.xy * 0.5f + 0.5f;
		uv.y = 1.0f - uv.y;
		float shadow = ShadowMap.SampleCmpLevelZero(ShadowSampler, uv, lightSpacePos.z);
		return shadow * (diffuse+specular) + ambient;
	}
	
	return float4(color,1.f) * (diffuse + specular) + ambient;
}

float4 PSMain( PS_IN input ) : SV_Target
{
	float3 viewDir = normalize(gInvView[3].xyz - input.worldPos);

	float4 materialDiffuse;
	float alpha;
	if(constantData.materialAlpha != -1)
	{
		materialDiffuse = DiffuseMap.Sample(Sampler, input.tex.xy);
		alpha = constantData.materialAlpha;
    }
	else
	{
		materialDiffuse = constantData.color;
		alpha = 32;
	}
	float4 result = CalcLight(viewDir, input.normal, input.worldPos, materialDiffuse, alpha, directionalLight.direction, directionalLight.intencity, float3(1.0f, 0.95f, 0.85f), true);
	
	for(int i = 0; i < 100; i++)
	{
		float3 lightDirection = input.worldPos-pointLight[i].position;
		float len = length(lightDirection);
		float attenuation = max(0.0f, 1-(len/pointLight[i].radius) );//(1-(lightDirection/pointLight.radius));
		attenuation *= attenuation;
		result += CalcLight(viewDir, input.normal, input.worldPos, materialDiffuse, alpha,  lightDirection, pointLight[i].intencity * attenuation, pointLight[i].color, false);
	}
	
	
	
	return result;
}
