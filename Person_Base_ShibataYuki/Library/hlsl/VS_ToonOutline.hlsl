//==========================================================
// [VS_ToonOutline]
//---------------------------------------------------------
//
//==========================================================

#include "common.hlsli"

struct VS_IN
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
};

struct VS_OUT
{
    float4 pos;
};


VS_OUT main(VS_IN vin) : SV_POSITION
{
    VS_OUT vout;

    vout.pos = float4(vin.pos, 1.0f);
    // ÉèÅ[ÉãÉhç¿ïW
    float4 wPos = mul(vout.pos, g_world);
    // ∂“◊Ç∆ÇÃãóó£
    float dist = length(wPos.xyz - g_vCameraPos.xyz);
    
    float margin = dist / 200.0f;
    vout.pos.xyz += normalize(vin.normal) * margin;
    vout.pos = mul(vout.pos, g_world);
    vout.pos = mul(vout.pos, g_view);
    vout.pos = mul(vout.pos, g_proj);
    
    
	return vout;
}