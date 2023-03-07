

#include <Common.hlsli>

// パラメータ
struct VS_OUTPUT
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
    float4 Diffuse : COLOR0;
};

float4 main(VS_OUTPUT pin) : SV_TARGET
{
    float4 color = g_ColorTexture.Sample(g_sampler, pin.TexCoord);
    
    // テクスチャ座標をずらして差分を計算
    float3 edge = color.rgb - g_ColorTexture.Sample(g_sampler, pin.TexCoord + float2(0.002f, 0)).rgb;
    
    edge += color.rgb - g_ColorTexture.Sample(g_sampler, pin.TexCoord + float2(-0.002f, 0)).rgb;
    edge += color.rgb - g_ColorTexture.Sample(g_sampler, pin.TexCoord + float2(0, 0.002f)).rgb;
    edge += color.rgb - g_ColorTexture.Sample(g_sampler, pin.TexCoord + float2(0, -0.002f)).rgb;
    
    // エッジ強度を上げるために倍率をかける
    edge = saturate(edge * 50.0f); 
    
    // エッジ部分を黒く、他は透明にする
    return float4(edge, 1.0f); 
}

