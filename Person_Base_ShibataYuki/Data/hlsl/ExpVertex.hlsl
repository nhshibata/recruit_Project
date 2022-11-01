// 爆発処理用頂点シェーダ [ExpVertex.hlsl]

// グローバル
cbuffer global : register(b0) {
	matrix g_mWVP;
	matrix g_mWorld;
};

// パラメータ
struct VS_INPUT {
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float4 Diffuse : COLOR;
};

struct VS_OUTPUT {
	float4 Position : SV_Position;
	float3 Pos4PS : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float4 Diffuse : COLOR;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
	float4 P = float4(input.Position, 1.0f);
	output.Position = mul(P, g_mWVP);
	output.Pos4PS = mul(P, g_mWorld).xyz;
	output.Normal = mul(float4(input.Normal, 0.0f), g_mWorld).xyz;
	output.Diffuse = input.Diffuse;
	return output;
}
