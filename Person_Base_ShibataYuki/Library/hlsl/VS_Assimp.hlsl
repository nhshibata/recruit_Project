//=========================================================
// [VS_Assimp.hlsl]
//---------------------------------------------------------
// 更新:2023/02/05 定数管理のため、 hlsli追加
// 更新:2023/02/06 名前変更
//---------------------------------------------------------
// Assimp用頂点シェーダ
//=========================================================

#include "common.hlsli"

// パラメータ
struct VS_INPUT
{
    float3 Pos : POSITION;
    float3 Normal : NORMAL;
    float2 Tex : TEXCOORD0;
    uint4 Bone : BONE_INDEX; // ボーンのインデックス
    float4 Weight : BONE_WEIGHT; // ボーンの重み
    
    uint id : SV_InstanceID; // インスタンスID
};

struct VS_OUTPUT
{
    float4 Pos : SV_Position;
    float2 Tex : TEXCOORD0;
    float3 Normal : TEXCOORD1;
    float3 PosForPS : TEXCOORD2;
    float4 SunPos : TEXCOORD3; // 太陽から見た位置
    
    uint id : SV_InstanceID; // インスタンスID
};

// スキニング後の頂点・法線
struct SKIN
{
    float4 Pos;
    float3 Norm;
};

// 頂点をスキニング (ボーンにより移動)
SKIN SkinVert(VS_INPUT input)
{
    SKIN output;
    float4 Pos = float4(input.Pos, 1.0f);
    float3 Norm = input.Normal;
    if (input.Weight.x == 0.0f)
    {
		// ボーン無し
        output.Pos = Pos;
        output.Norm = Norm;
    }
    else
    {
		// ボーン0
        uint uBone = input.Bone.x;
        float fWeight = input.Weight.x;
        matrix m = g_BoneWorld[uBone];
        output.Pos = fWeight * mul(Pos, m);
        output.Norm = fWeight * mul(Norm, (float3x3) m);
		// ボーン1
        uBone = input.Bone.y;
        fWeight = input.Weight.y;
        m = g_BoneWorld[uBone];
        output.Pos += fWeight * mul(Pos, m);
        output.Norm += fWeight * mul(Norm, (float3x3) m);
		// ボーン2
        uBone = input.Bone.z;
        fWeight = input.Weight.z;
        m = g_BoneWorld[uBone];
        output.Pos += fWeight * mul(Pos, m);
        output.Norm += fWeight * mul(Norm, (float3x3) m);
		// ボーン3
        uBone = input.Bone.w;
        fWeight = input.Weight.w;
        m = g_BoneWorld[uBone];
        output.Pos += fWeight * mul(Pos, m);
        output.Norm += fWeight * mul(Norm, (float3x3) m);
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
    float4x4 mWorld = mul(g_mtxWorld, g_Instancing[input.id].mWorld);
    output.Pos = mul(vSkinned.Pos, mul(mWorld, g_mtxVP));
    output.Tex = mul(float4(input.Tex, 0.0f, 1.0f), g_mtxTexture).xy;
    output.Normal = mul(vSkinned.Norm, (float3x3) mWorld);
    output.PosForPS = mul(vSkinned.Pos, mWorld).xyz;
    
    // 太陽用頂点計算
    //float4 wPos = mul(float4(input.Pos, 1.0f), mWorld);
    float4 wPos = mul(float4(input.Pos, 1.0f), g_Instancing[input.id].mWorld);
    wPos = mul(wPos, sunView);
    wPos = mul(wPos, sunProj);
    output.SunPos = wPos;
    
    output.id = input.id;
    
    return output;
}
