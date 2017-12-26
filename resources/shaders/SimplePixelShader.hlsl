Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

struct VertexShaderOutput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXTCOORD0;
};

float4 SimplePixelShader (VertexShaderOutput IN) : SV_TARGET
{
	return txDiffuse.Sample (samLinear, IN.texcoord) * IN.color;
}