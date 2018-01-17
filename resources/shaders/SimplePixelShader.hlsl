Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

cbuffer LightBuffer
{
    float4 diffuseColor;
    float3 lightDirection;
    float padding;
};

struct VertexShaderOutput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXTCOORD0;
};

float4 SimplePixelShader (VertexShaderOutput IN) : SV_TARGET
{
	float4 textureColor;
    float3 lightDir;
    float lightIntensity;
    float4 color;

	return txDiffuse.Sample (samLinear, IN.texcoord) * IN.color;
}