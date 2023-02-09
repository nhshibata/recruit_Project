//=========================================================
// [PS_Toon]
//---------------------------------------------------------
// 
//=========================================================

#include "common.hlsli"

struct PS_IN
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL0;
    float2 uv : TEXCOORD0;
};

Texture2D Tex : register(t0);
Texture2D rampTex : register(t1);
SamplerState g_samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
    // 通常ﾃｸｽﾁｬ取得
    float4 color = Tex.Sample(g_samp, pin.uv);
    
    // 内積
    // 同じ方向のベクトル同士なら正の値
    // 垂直方向のベクトル同士なら0
    // 反対方向のベクトル同士なら負の値
    
    // オブジェクトの法線とライトのベクトルから内積を計算
    // そのまま計算すると明るくなる部分が負の値になるので、
    //「あえて」ライトのベクトルを反転させる
    float3 N = normalize(pin.normal);
    float3 L = normalize(-g_vLightDir.xyz);
    
    // 拡散光(diffuse)物体の表面に直接あたる光、物体の色を表す
    float diffuse = dot(N, L);

    // 内積の値を基にランプﾃｸｽﾁｬの色を引っ張り出す
    // ※内積の結果は-1～1なので0～1に変換する
    diffuse = diffuse * 0.5f + 0.5f;
    
    float4 toonColor = rampTex.Sample(g_samp, float2(diffuse, 0));
    
    color.rgb *= toonColor.rgb;
    
    return color;
}

