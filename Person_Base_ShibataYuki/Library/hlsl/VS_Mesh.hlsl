//=========================================================
// [VS_Mesh]
//---------------------------------------------------------
// 更新:2023/02/05 定数管理のため、 hlsli追加
// 更新:2023/02/06 名前変更
//---------------------------------------------------------
// 頂点シェーダインスタンシング用
// インスタンシング前提
//=========================================================

#include "meshCommon.hlsli"


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

    MeshOutput work = CalcMesh(input, g_Instancing[id].mWorld, g_view, g_proj);
    output.Position = work.Position;
    output.Pos4PS = work.Pos4PS;
    output.Normal = work.Normal;
    output.TexCoord = work.TexCoord;
    output.Diffuse = work.Diffuse;

     // 太陽用頂点計算
    float4 sunPos = float4(input.Position.xyz, 1.0f);
    sunPos = mul(sunPos, g_Instancing[id].mWorld);
    sunPos = mul(sunPos, g_sunView);
    sunPos = mul(sunPos, g_sunProj);
    output.SunPos = sunPos;
    
    output.id = id;
    
	return output;
}
