//=========================================================
// [PS_Toon]
//---------------------------------------------------------
// 
//=========================================================

#include "common.hlsli"

struct PS_IN
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL0;
    float2 uv : TEXCOORD0;
};

Texture2D Tex : register(t0);
Texture2D rampTex : register(t1);
SamplerState g_samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
    // �ʏ�ø����擾
    float4 color = Tex.Sample(g_samp, pin.uv);
    
    // ����
    // ���������̃x�N�g�����m�Ȃ琳�̒l
    // ���������̃x�N�g�����m�Ȃ�0
    // ���Ε����̃x�N�g�����m�Ȃ畉�̒l
    
    // �I�u�W�F�N�g�̖@���ƃ��C�g�̃x�N�g��������ς��v�Z
    // ���̂܂܌v�Z����Ɩ��邭�Ȃ镔�������̒l�ɂȂ�̂ŁA
    //�u�����āv���C�g�̃x�N�g���𔽓]������
    float3 N = normalize(pin.normal);
    float3 L = normalize(-g_vLightDir.xyz);
    
    // �g�U��(diffuse)���̂̕\�ʂɒ��ڂ�������A���̂̐F��\��
    float diffuse = dot(N, L);

    // ���ς̒l����Ƀ����vø����̐F����������o��
    // �����ς̌��ʂ�-1�`1�Ȃ̂�0�`1�ɕϊ�����
    diffuse = diffuse * 0.5f + 0.5f;
    
    float4 toonColor = rampTex.Sample(g_samp, float2(diffuse, 0));
    
    color.rgb *= toonColor.rgb;
    
    return color;
}

