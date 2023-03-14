//==========================================================
// [DepthOfField.hlsl]
//---------------------------------------------------------
// 被写界深度
//==========================================================

#include <Common.hlsli>

// 頂点シェーダーへの入力
struct VSIn
{
    float4 pos : POSITION; // モデルの頂点座標
    float3 normal : NORMAL; // 法線
    float3 tangent : TANGENT;
    float3 biNormal : BINORMAL;
    float2 uv : TEXCOORD0; // UV座標
};

// ピクセルシェーダーへの入力
struct PSIn
{
    float4 pos : SV_POSITION; // スクリーン空間でのピクセルの座標
    float3 normal : NORMAL; // 法線
    float3 tangent : TANGENT; // 接ベクトル
    float3 biNormal : BINORMAL; // 従法線ベクトル
    float2 uv : TEXCOORD0; // UV座標
    float3 worldPos : TEXCOORD1; // ワールド空間でのピクセルの座標

    // step-7 カメラ空間でのZ値を記録する変数を追加
    float3 depthInView : TEXCOORD2; // カメラ空間でのZ値
};


PSIn main(VSIn vin)
{
    PSIn pout;

    //pout.pos = mul(g_world, vin.pos);
    //pout.worldPos = pout.pos;
    //pout.worldPos = mul(g_view, pout.pos);
    
    //pout.pos = mul(g_proj, pout.pos);
    
    //// 4x3に変換
    //float4x3 mWorld = (float4x3)g_world;
    //pout.normal = normalize(mul(mWorld, vin.normal));
    //pout.tangent = normalize(mul(mWorld, vin.tangent));
    //pout.biNormal = normalize(mul(mWorld, vin.biNormal));
    //pout.uv = vin.uv;
    
	return pout;
}