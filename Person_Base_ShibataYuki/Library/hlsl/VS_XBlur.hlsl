
#include <Common.hlsli>

// 横ブラー頂点
struct VSInput
{
    float3 pos : POSITION0;
    float4 color : COLOR0;
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

PS_BlurInput main(VSInput vin)
{
    PS_BlurInput vout;
    
    float4 P = mul(float4(vin.pos, 1.0f), g_world);
    P = mul(P, g_view);
    vout.pos = mul(P, g_proj);
    
    // ﾃｸｽﾁｬ
    float2 texSize;
    float level;
    g_texture.GetDimensions(0, texSize.x, texSize.y, level);
    
    // UV保存
    float2 tex = vin.uv;
    
    texSize.x = 1280.0f / 2;
    texSize.y = 720.0f;
    
    // テクセルからU座標を1テクセルずらすためのオフセット計算
    vout.tex0.xy = float2(1.0f / texSize.x, 0.0f);
    // テクセルからU座標を3テクセルずらすためのオフセット計算
    vout.tex1.xy = float2(3.0f / texSize.x, 0.0f);
    // テクセルからU座標を5テクセルずらすためのオフセット計算
    vout.tex2.xy = float2(5.0f / texSize.x, 0.0f);
    // テクセルからU座標を7テクセルずらすためのオフセット計算
    vout.tex3.xy = float2(7.0f / texSize.x, 0.0f);
    // テクセルからU座標を9テクセルずらすためのオフセット計算
    vout.tex4.xy = float2(9.0f / texSize.x, 0.0f);
    // テクセルからU座標を11テクセルずらすためのオフセット計算
    vout.tex5.xy = float2(11.0f / texSize.x, 0.0f);
    // テクセルからU座標を13テクセルずらすためのオフセット計算
    vout.tex6.xy = float2(13.0f / texSize.x, 0.0f);
    // テクセルからU座標を15テクセルずらすためのオフセット計算
    vout.tex7.xy = float2(15.0f / texSize.x, 0.0f);
    
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