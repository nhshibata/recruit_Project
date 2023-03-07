

#include <Common.hlsli>

// �p�����[�^
struct VS_OUTPUT
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
    float4 Diffuse : COLOR0;
};

float4 main(VS_OUTPUT pin) : SV_TARGET
{
    float4 color = g_ColorTexture.Sample(g_sampler, pin.TexCoord);
    
    // �e�N�X�`�����W�����炵�č������v�Z
    float3 edge = color.rgb - g_ColorTexture.Sample(g_sampler, pin.TexCoord + float2(0.002f, 0)).rgb;
    
    edge += color.rgb - g_ColorTexture.Sample(g_sampler, pin.TexCoord + float2(-0.002f, 0)).rgb;
    edge += color.rgb - g_ColorTexture.Sample(g_sampler, pin.TexCoord + float2(0, 0.002f)).rgb;
    edge += color.rgb - g_ColorTexture.Sample(g_sampler, pin.TexCoord + float2(0, -0.002f)).rgb;
    
    // �G�b�W���x���グ�邽�߂ɔ{����������
    edge = saturate(edge * 50.0f); 
    
    // �G�b�W�����������A���͓����ɂ���
    return float4(edge, 1.0f); 
}

