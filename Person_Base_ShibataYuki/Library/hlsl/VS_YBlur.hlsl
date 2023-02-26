
#include <Common.hlsli>

// �O���[�o��
cbuffer global : register(b0)
{
    matrix g_mWorld;
    matrix g_mView;
    matrix g_mProjection;
    matrix g_mtxTexture;
};

// �u���[���_
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

// �c�u���[
PS_BlurInput main(VSInput vin) 
{
    PS_BlurInput vout;
    
    float4 P = mul(float4(vin.pos, 1.0f), g_mWorld);
    P = mul(P, g_mView);
    vout.pos = mul(P, g_mProjection);
    
    // ø���
    float2 texSize;
    float level;
    g_mainTexture.GetDimensions(0, texSize.x, texSize.y, level);
    
    // UV�ۑ�
    float2 tex = vin.uv;
    
    // �e�N�Z������U���W��1�e�N�Z�����炷���߂̃I�t�Z�b�g�v�Z
    vout.tex0.xy = float2(0.0f, 1.0f / texSize.x);
    // �e�N�Z������U���W��3�e�N�Z�����炷���߂̃I�t�Z�b�g�v�Z
    vout.tex1.xy = float2(0.0f, 3.0f / texSize.x);
    // �e�N�Z������U���W��5�e�N�Z�����炷���߂̃I�t�Z�b�g�v�Z
    vout.tex2.xy = float2(0.0f, 5.0f / texSize.x);
    // �e�N�Z������U���W��7�e�N�Z�����炷���߂̃I�t�Z�b�g�v�Z
    vout.tex3.xy = float2(0.0f, 7.0f / texSize.x);
    // �e�N�Z������U���W��9�e�N�Z�����炷���߂̃I�t�Z�b�g�v�Z
    vout.tex4.xy = float2(0.0f, 9.0f / texSize.x);
    // �e�N�Z������U���W��11�e�N�Z�����炷���߂̃I�t�Z�b�g�v�Z
    vout.tex5.xy = float2(0.0f, 11.0f / texSize.x);
    // �e�N�Z������U���W��13�e�N�Z�����炷���߂̃I�t�Z�b�g�v�Z
    vout.tex6.xy = float2(0.0f, 13.0f / texSize.x);
    // �e�N�Z������U���W��15�e�N�Z�����炷���߂̃I�t�Z�b�g�v�Z
    vout.tex7.xy = float2(0.0f, 15.0f / texSize.x);
    
    // �I�t�Z�b�g��-1���|���ă}�C�i�X�����̃I�t�Z�b�g���v�Z
    vout.tex0.zw = vout.tex0.xy * -1.0f;
    vout.tex1.zw = vout.tex1.xy * -1.0f;
    vout.tex2.zw = vout.tex2.xy * -1.0f;
    vout.tex3.zw = vout.tex3.xy * -1.0f;
    vout.tex4.zw = vout.tex4.xy * -1.0f;
    vout.tex5.zw = vout.tex5.xy * -1.0f;
    vout.tex6.zw = vout.tex6.xy * -1.0f;
    vout.tex7.zw = vout.tex7.xy * -1.0f;
    
    // �I�t�Z�b�g�Ƀe�N�Z����UV���W�𑫂��Z���āA���ۂ̃T���v��������UV���W�ɕϊ�
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