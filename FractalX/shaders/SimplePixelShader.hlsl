Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

struct PixelShaderInput
{
	float4 Pos : SV_POSITION;
	float4 color : COLOR;
	float2 Tex : TEXCOORD;
};

float4 SimplePixelShader (PixelShaderInput IN) : SV_TARGET
{
	return txDiffuse.Sample (samLinear, IN.Tex) * IN.color;
}