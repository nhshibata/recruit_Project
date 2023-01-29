// 頂点シェーダインスタンシング用

#define MAX_DATA        1024

struct InstancingData
{
    matrix mWorld;
};

// グローバル
cbuffer global : register(b0) {
	matrix g_mVP;           // ワールド×ビュー×射影行列
	matrix g_mWorld;        // ワールド行列
    matrix g_mTexture; 
};

cbuffer InstancingGlobal : register(b3)
{
    InstancingData g_Instancing[MAX_DATA];
}

// 太陽の位置にあるｶﾒﾗに表示するための行列
cbuffer SunCamera : register(b4)
{
    float4x4 sunView;
    float4x4 sunProj;
};

// パラメータ
struct VS_INPUT {
	float3	Position	: POSITION;
	float3	Normal		: NORMAL;
	float2	TexCoord	: TEXCOORD0;
	float4	Diffuse		: COLOR0;
    uint instID : SV_InstanceID;
};

struct VS_OUTPUT {
	float4	Position	: SV_Position;
	float3	Pos4PS		: TEXCOORD0;
	float3	Normal		: TEXCOORD1;
	float2	TexCoord	: TEXCOORD2;
	float4	Diffuse		: COLOR0;
    float4  SunPos      : TEXCOORD3; // 太陽から見た位置
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
    uint id = input.instID % MAX_DATA;

    float4 P = float4(input.Position, 1.0f);
    float4 wPos = mul(P, g_Instancing[id].mWorld);
    output.Position = mul(wPos,g_mVP);
    output.Pos4PS = wPos.xyz;
	//output.Normal = mul(float4(input.Normal, 0.0f), g_mWorld).xyz;
    output.Normal = mul(float4(input.Normal, 0.0f), g_Instancing[id].mWorld).xyz;
	//output.Normal = input.Normal.xyz;
	output.TexCoord = mul(float4(input.TexCoord, 0.0f, 1.0f), g_mTexture).xy;
	output.Diffuse = input.Diffuse;

     // 太陽用頂点計算
    float4 sunPos = mul(float4(input.Position.xyz, 1.0f), g_mWorld);
    sunPos = mul(sunPos, sunView);
    sunPos = mul(sunPos, sunProj);
    output.SunPos = sunPos;
    
	return output;
}
