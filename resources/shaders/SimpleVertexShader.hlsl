cbuffer PerApplication : register(b0)
{
	matrix projectionMatrix;
}

cbuffer PerFrame : register(b1)
{
	matrix viewMatrix;
}

cbuffer PerObject : register(b2)
{
	matrix worldMatrix;
}

struct AppData
{
	float3 position : POSITION;
	float3 color: COLOR;
	float2 texcoord : TEXCOORD0;
};

struct VertexShaderOutput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXTCOORD0;
};

VertexShaderOutput SimpleVertexShader (AppData IN)
{
	VertexShaderOutput OUT;

	matrix mvp = mul (projectionMatrix, mul (viewMatrix, worldMatrix));
	OUT.position = mul (mvp, float4(IN.position, 1.0f));
	OUT.color = float4(IN.color, 1.0f);
	OUT.texcoord = IN.texcoord;

	return OUT;
}