

Texture2D txDiffuse;
SamplerState samLinear;

cbuffer LightBuffer
{
	float4 ambient;
	float4 diffuse;
};

struct VertexShaderOutput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXTCOORD0;
	float3 normal : NORMAL;
};

float4 SimplePixelShader (VertexShaderOutput IN) : SV_TARGET
{
	float4 diffuse = txDiffuse.Sample (samLinear, IN.texcoord) * float4(1.0,0.0,0,1);

	return  diffuse;
}