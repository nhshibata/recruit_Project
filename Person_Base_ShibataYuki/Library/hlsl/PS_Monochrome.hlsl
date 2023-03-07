

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
 
     // ����
    // ���������̃x�N�g�����m�Ȃ琳�̒l
    // ���������̃x�N�g�����m�Ȃ�0
    // ���Ε����̃x�N�g�����m�Ȃ畉�̒l
    
    // �I�u�W�F�N�g�̖@���ƃ��C�g�̃x�N�g��������ς��v�Z
    // ���̂܂܌v�Z����Ɩ��邭�Ȃ镔�������̒l�ɂȂ�̂ŁA
    //�u�����āv���C�g�̃x�N�g���𔽓]������
    float4 normal = g_NormalTexture.Sample(g_sampler, pin.TexCoord);
    float3 N = normalize(normal.xyz);
    float3 L = normalize(-g_vLightDir.xyz);
    
    // �g�U��(diffuse)���̂̕\�ʂɒ��ڂ�������A���̂̐F��\��
    float diffuse = dot(N, L);

    // ���ς̒l����Ƀ����vø����̐F����������o��
    // �����ς̌��ʂ�-1�`1�Ȃ̂�0�`1�ɕϊ�����
    diffuse = diffuse * 0.5f + 0.5f;
    
    float4 toonColor = g_rampTexture.Sample(g_sampler, float2(diffuse, 0));
    
    color.rgb *= toonColor.rgb;
 
    return color;
}