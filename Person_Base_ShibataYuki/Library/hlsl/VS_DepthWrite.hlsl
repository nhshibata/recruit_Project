//==========================================================
// [VS_DepthWrite]
//---------------------------------------------------------
// 作成:2023/01/22
// 更新:2023/01/26 instanceID追加
// 更新:2023/02/05 定数管理のため、 hlsli追加
// 更新:2023/02/06 名前変更
//---------------------------------------------------------
// Z値書き込みDepthWritePSに渡される
// InstancingVertex(model,mesh)にその後渡される
//==========================================================

#include <Common.hlsli>

struct VS_IN_Depth
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    
    uint id : SV_InstanceID; // インスタンスID
};

struct VS_OUT
{
    float4 pos : SV_POSITION;
    float4 screenPos : TEXCOORD0;
};


VS_OUT main(VS_IN_Depth vin)
{
    VS_OUT vout;
    
    //--- ローカル座標 >ワールド座標 > ビュー座標 >プロジェクション座標 と変換をかける
    
    //float4x4 mWorld = mul(world, g_World[vin.id]);
    
    // インスタンシングごとのﾛｰｶﾙ座標からスクリーン座標へ
    float4x4 mWorld = g_Instancing[vin.id].mWorld;
    vout.pos = float4(vin.pos, 1.0f);
    vout.pos = mul(vout.pos, mWorld);
    vout.pos = mul(vout.pos, g_sunView);
    vout.pos = mul(vout.pos, g_sunProj);
    
    // SV_POSITIONとは別にﾃﾞｰﾀを渡す
    vout.screenPos = vout.pos;
    
    return vout;
}