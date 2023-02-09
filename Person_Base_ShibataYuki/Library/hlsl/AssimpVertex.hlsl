//=========================================================
// Assimp用頂点シェーダ (AssimpVertex.hlsl)
//---------------------------------------------------------
// 更新:2023/02/05 定数管理のため、 hlsli追加
//=========================================================

#include "common.hlsli"

// パラメータ
struct VS_INPUT {
	float3	Pos		: POSITION;
	float3	Normal	: NORMAL;
	float2	Tex		: TEXCOORD0;
	uint4	Bone	: BONE_INDEX;	// ボーンのインデックス
	float4	Weight	: BONE_WEIGHT;	// ボーンの重み
    
};

struct VS_OUTPUT {
	float4	Pos			: SV_Position;
	float2	Tex			: TEXCOORD0;
	float3	Normal		: TEXCOORD1;
	float3	PosForPS	: TEXCOORD2;
};

// スキニング後の頂点・法線
struct SKIN {
	float4 Pos;
	float3 Norm;
};

// 頂点をスキニング (ボーンにより移動)
SKIN SkinVert(VS_INPUT input)
{
	SKIN output;
	float4 Pos = float4(input.Pos, 1.0f);
	float3 Norm = input.Normal;
	if (input.Weight.x == 0.0f) {
		// ボーン無し
		output.Pos = Pos;
		output.Norm = Norm;
	} else {
		// ボーン0
		uint uBone = input.Bone.x;
		float fWeight = input.Weight.x;
		matrix m = g_BoneWorld[uBone];
		output.Pos = fWeight * mul(Pos, m);
		output.Norm = fWeight * mul(Norm, (float3x3)m);
		// ボーン1
		uBone = input.Bone.y;
		fWeight = input.Weight.y;
		m = g_BoneWorld[uBone];
		output.Pos += fWeight * mul(Pos, m);
		output.Norm += fWeight * mul(Norm, (float3x3)m);
		// ボーン2
		uBone = input.Bone.z;
		fWeight = input.Weight.z;
		m = g_BoneWorld[uBone];
		output.Pos += fWeight * mul(Pos, m);
		output.Norm += fWeight * mul(Norm, (float3x3)m);
		// ボーン3
		uBone = input.Bone.w;
		fWeight = input.Weight.w;
		m = g_BoneWorld[uBone];
		output.Pos += fWeight * mul(Pos, m);
		output.Norm += fWeight * mul(Norm, (float3x3)m);
	}
	return output;
}

//
// 頂点シェーダ
//
VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
	SKIN vSkinned = SkinVert(input);

    matrix VP = mul(g_view, g_proj); 
    // ﾜｰﾙﾄﾞ×ビュー×プロジェクション作成
    output.Pos = mul(vSkinned.Pos, mul(g_world, VP));
	output.Tex = mul(float4(input.Tex, 0.0f, 1.0f), g_mTexture).xy;
	output.Normal = mul(vSkinned.Norm, (float3x3)g_world);
	output.PosForPS = mul(vSkinned.Pos, g_world).xyz;

	return output;
}
