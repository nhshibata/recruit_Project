
#include <Common.hlsli>

struct PS_BlurInput
{
    float4 pos : SV_POSITION;
    float4 tex0 : TEXCOORD0;
    float4 tex1 : TEXCOORD1;
    float4 tex2 : TEXCOORD2;
    float4 tex3 : TEXCOORD3;
    float4 tex4 : TEXCOORD4;
    float4 tex5 : TEXCOORD5;
    float4 tex6 : TEXCOORD6;
    float4 tex7 : TEXCOORD7;
};

// ps
// 横ブラー
float4 main(PS_BlurInput pin) : SV_Target
{
    float4 color;
    
    // テクセルから+に8テクセル、重みつきでサンプル
    color = g_weight[0].x * g_texture.Sample(g_sampler, pin.tex0.xy);
    color += g_weight[0].y * g_texture.Sample(g_sampler, pin.tex1.xy);
    color += g_weight[0].z * g_texture.Sample(g_sampler, pin.tex2.xy);
    color += g_weight[0].w * g_texture.Sample(g_sampler, pin.tex3.xy);
    color += g_weight[1].x * g_texture.Sample(g_sampler, pin.tex4.xy);
    color += g_weight[1].y * g_texture.Sample(g_sampler, pin.tex5.xy);
    color += g_weight[1].z * g_texture.Sample(g_sampler, pin.tex6.xy);
    color += g_weight[1].w * g_texture.Sample(g_sampler, pin.tex7.xy);
    
    // テクセルから-に8テクセル、重みつきでサンプル
    color += g_weight[0].x * g_texture.Sample(g_sampler, pin.tex0.zw);
    color += g_weight[0].y * g_texture.Sample(g_sampler, pin.tex1.zw);
    color += g_weight[0].z * g_texture.Sample(g_sampler, pin.tex2.zw);
    color += g_weight[0].w * g_texture.Sample(g_sampler, pin.tex3.zw);
    color += g_weight[1].x * g_texture.Sample(g_sampler, pin.tex4.zw);
    color += g_weight[1].y * g_texture.Sample(g_sampler, pin.tex5.zw);
    color += g_weight[1].z * g_texture.Sample(g_sampler, pin.tex6.zw);
    color += g_weight[1].w * g_texture.Sample(g_sampler, pin.tex7.zw);

    return float4(color.rgb, 1.0f);
}