
#include <Common.hlsli>

// グローバル
cbuffer global : register(b0)
{
    matrix g_mWorld;
    matrix g_mView;
    matrix g_mProjection;
    matrix g_mtxTexture;
};

// ブラー頂点
struct VSInput
{
    float3 pos : POSITION0;
    float2 uv : TEXCOORD0;
};
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

// 縦ブラー
PS_BlurInput main(VSInput vin) 
{
    PS_BlurInput vout;
    
    float4 P = mul(float4(vin.pos, 1.0f), g_mWorld);
    P = mul(P, g_mView);
    vout.pos = mul(P, g_mProjection);
    
    // ﾃｸｽﾁｬ
    float2 texSize;
    float level;
    g_mainTexture.GetDimensions(0, texSize.x, texSize.y, level);
    
    // UV保存
    float2 tex = vin.uv;
    
    // テクセルからU座標を1テクセルずらすためのオフセット計算
    vout.tex0.xy = float2(0.0f, 1.0f / texSize.x);
    // テクセルからU座標を3テクセルずらすためのオフセット計算
    vout.tex1.xy = float2(0.0f, 3.0f / texSize.x);
    // テクセルからU座標を5テクセルずらすためのオフセット計算
    vout.tex2.xy = float2(0.0f, 5.0f / texSize.x);
    // テクセルからU座標を7テクセルずらすためのオフセット計算
    vout.tex3.xy = float2(0.0f, 7.0f / texSize.x);
    // テクセルからU座標を9テクセルずらすためのオフセット計算
    vout.tex4.xy = float2(0.0f, 9.0f / texSize.x);
    // テクセルからU座標を11テクセルずらすためのオフセット計算
    vout.tex5.xy = float2(0.0f, 11.0f / texSize.x);
    // テクセルからU座標を13テクセルずらすためのオフセット計算
    vout.tex6.xy = float2(0.0f, 13.0f / texSize.x);
    // テクセルからU座標を15テクセルずらすためのオフセット計算
    vout.tex7.xy = float2(0.0f, 15.0f / texSize.x);
    
    // オフセットに-1を掛けてマイナス方向のオフセットも計算
    vout.tex0.zw = vout.tex0.xy * -1.0f;
    vout.tex1.zw = vout.tex1.xy * -1.0f;
    vout.tex2.zw = vout.tex2.xy * -1.0f;
    vout.tex3.zw = vout.tex3.xy * -1.0f;
    vout.tex4.zw = vout.tex4.xy * -1.0f;
    vout.tex5.zw = vout.tex5.xy * -1.0f;
    vout.tex6.zw = vout.tex6.xy * -1.0f;
    vout.tex7.zw = vout.tex7.xy * -1.0f;
    
    // オフセットにテクセルのUV座標を足し算して、実際のサンプリンするUV座標に変換
    vout.tex0 += float4(tex, tex);
    vout.tex1 += float4(tex, tex);
    vout.tex2 += float4(tex, tex);
    vout.tex3 += float4(tex, tex);
    vout.tex4 += float4(tex, tex);
    vout.tex5 += float4(tex, tex);
    vout.tex6 += float4(tex, tex);
    vout.tex7 += float4(tex, tex);
 
    return vout;
}