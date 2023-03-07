//==========================================================
// [PS_Luminance]
//---------------------------------------------------------
// �P�x���o
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
    // GBuffer��Color�擾
    float4 color = g_ColorTexture.Sample(g_sampler, pin.uv);
    //float4 color = g_texture.Sample(g_sampler, pin.uv);
    
    // �T���v�����O�����J���[�̖��邳���v�Z
    float t = dot(color.xyz, float3(0.2125f, 0.7154f, 0.0721));

    // 1�ȉ��Ȃ�
    //clip(t - 1.0f);
    clip(t - g_rate.x);

	return color;  
}

