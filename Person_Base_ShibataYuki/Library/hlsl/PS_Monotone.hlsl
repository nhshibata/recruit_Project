//==========================================================
//[PS_Monotone]
//---------------------------------------------------------
// モノトーン/セピア
//==========================================================

cbuffer Monotone
{
    float monotoneRate;
    float3 dummy;
};

struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

Texture2D renderTex : register(t0);
SamplerState g_sampler : register(s0); // サンプラ

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = renderTex.Sample(g_sampler, pin.uv);
    
#if 1
    float monotone = 0.299f * color.r + 0.587f * color.g + 0.114f * color.b;
    color = lerp(color, monotone, monotoneRate);
#else
    // セピア変換
	float mono = color.r * 0.3f + color.g * 0.6f + color.b * 0.3f;
	color.rgb = mono;

	color.r *= 1.0f;	// 赤色はそのまま
	color.g *= 0.7f;	// その他の成分を減らす
	color.b *= 0.4f;	// 青色は暗い色で変化がわかりづらいので大きく減らす
#endif
        
    return color;
}

