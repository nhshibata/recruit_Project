//==========================================================
// [PS_ColorTexture]
//---------------------------------------------------------
//
//==========================================================

#include <Common.hlsli>

struct PS_IN
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

float4 main(PS_IN pin) : SV_TARGET
{
    return g_ColorTexture.Sample(g_sampler, pin.uv);
}