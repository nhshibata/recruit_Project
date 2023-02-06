//=========================================================
// [VS_Mesh]
//---------------------------------------------------------
// 更新:2023/02/05 定数管理のため、 hlsli追加
// 更新:2023/02/06 名前変更
//---------------------------------------------------------
// 頂点シェーダインスタンシング用
// インスタンシング前提
//=========================================================

#include "common.hlsli"

// パラメータ
struct VS_INPUT {
	float3	Position	: POSITION;
	float3	Normal		: NORMAL;
	float2	TexCoord	: TEXCOORD0;
	float4	Diffuse		: COLOR0;
    
    uint id : SV_InstanceID; // インスタンスID
};

struct VS_OUTPUT {
	float4	Position	: SV_Position;
	float3	Pos4PS		: TEXCOORD0;
	float3	Normal		: TEXCOORD1;
	float2	TexCoord	: TEXCOORD2;
	float4	Diffuse		: COLOR0;
    float4  SunPos      : TEXCOORD3; // 太陽から見た位置
    
    uint id : SV_InstanceID; // インスタンスID
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
    uint id = input.id % MAX_WORLD_MATRIX;

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
    
    output.id = id;
    
	return output;
}
