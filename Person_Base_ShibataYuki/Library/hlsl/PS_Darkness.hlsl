//=========================================================
// []
// 
//=========================================================

#include <Common.hlsli>

// ÉpÉâÉÅÅ[É^
struct PS_IN
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
    float4 Diffuse : COLOR0;
};

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = g_ColorTexture.Sample(g_sampler, pin.TexCoord);

    clip(color.a - 1.0f);
    
	return color;
}