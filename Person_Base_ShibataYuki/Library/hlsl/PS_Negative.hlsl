

#include <Common.hlsli>

// ÉpÉâÉÅÅ[É^
struct VS_OUTPUT
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
    float4 Diffuse : COLOR0;
};

float4 main(VS_OUTPUT pin) : SV_TARGET
{
    float4 Diff = g_ColorTexture.Sample(g_sampler, pin.TexCoord);
    
    float4 color = Diff;
    float3 negaColor;
    negaColor.r = 1.0f - color.r;
    negaColor.g = 1.0f - color.g;
    negaColor.b = 1.0f - color.b;
    Diff.rgb = lerp(color.rgb, negaColor, g_rate.r);
    
    return Diff;
}

