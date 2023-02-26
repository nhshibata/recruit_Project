//==========================================================
// [PS_Mix2DColor]
//---------------------------------------------------------
// 色画像3合わせ
// PostProcess画像を描画するか確認する
// 2D画像から複数のレイヤーの奥行きは判断できないため
// GBufferの色画像とポストプロセス前の画像を比較し、差がなければ描画されたと判定する
//==========================================================

#include <common.hlsli>

struct PS_IN
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = g_ColorTexture.Sample(g_sampler, pin.uv);    // GBufferカラーﾃｸｽﾁｬ
    float4 mainColor = g_texture.Sample(g_sampler, pin.uv);     // ﾒｲﾝﾃｸｽﾁｬ
    float4 postColor = g_postTexture.Sample(g_sampler, pin.uv); // ポストプロセス後ﾃｸｽﾁｬ
    
    // 描画された色とGBufferに書き込まれた色
    float4 check = mainColor - color;
    
    // 差が0ではないか確認
    float disp = any(check);
    // 差がある = 1.0fならピクセルキル
    clip(-disp);

    return postColor;
}