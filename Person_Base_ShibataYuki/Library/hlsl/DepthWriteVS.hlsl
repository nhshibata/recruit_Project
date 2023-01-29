//==========================================================
// [DepthWriteVS]
//---------------------------------------------------------
// 作成:2023/01/22
// 更新:2023/01/26 instanceID追加
//---------------------------------------------------------
// Z値書き込みDepthWritePSに渡される
// InstancingVertex(model,mesh)にその後渡される
//==========================================================

#define MAX_WORLD_MATRIX 1024

struct VS_IN
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

// ワールド行列配列
cbuffer global_instance : register(b3)
{
    matrix g_World[MAX_WORLD_MATRIX];
};

cbuffer WVP : register(b5)
{
    float4x4 world;
    float4x4 view;
    float4x4 proj;
};

VS_OUT main(VS_IN vin)
{
    VS_OUT vout;
    
    //--- ローカル座標 >ワールド座標 > ビュー座標 >プロジェクション座標 と変換をかける
    
    //float4x4 mWorld = mul(world, g_World[vin.id]);
    
    // インスタンシングごとのﾛｰｶﾙ座標からスクリーン座標へ
    float4x4 mWorld = g_World[vin.id];
    vout.pos = float4(vin.pos, 1.0f);
    vout.pos = mul(vout.pos, mWorld);
    vout.pos = mul(vout.pos, view);
    vout.pos = mul(vout.pos, proj);
    
    // SV_POSITIONとは別にﾃﾞｰﾀを渡す
    vout.screenPos = vout.pos;
    
    return vout;
}