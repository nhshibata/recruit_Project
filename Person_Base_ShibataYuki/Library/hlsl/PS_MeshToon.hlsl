//=========================================================
// [PS_MeshToon]
//---------------------------------------------------------
// �쐬:2023/02/09
//---------------------------------------------------------
// �K��
//=========================================================

#include "meshCommon.hlsli"

// �p�����[�^
struct VS_OUTPUT
{
    float4 Position : SV_Position;
    float3 Pos4PS : TEXCOORD0;
    float3 Normal : TEXCOORD1;
    float2 TexCoord : TEXCOORD2;
    float4 Diffuse : COLOR0;
    float4 SunPos : TEXCOORD3; // ���z���猩���ʒu
    
    uint id : SV_InstanceID; // �C���X�^���XID
};

float4 main(VS_OUTPUT input) : SV_Target0
{
    // mesh�̋��ʊ֐�
    MeshOutput vin;
    vin.Position = input.Position;
    vin.Pos4PS = input.Pos4PS;
    vin.Diffuse = input.Diffuse;
    vin.TexCoord = input.TexCoord;
    vin.Normal = input.Normal;
    float4 Diff = CalcPSMaterial(vin, g_material[input.id].fAmbient, g_material[input.id].fDiffuse,
                    g_material[input.id].fSpecular, g_material[input.id].fEmissive, g_material[input.id].fFlags);
    
     // �ʏ�ø����擾
    //float4 color = g_texture.Sample(g_sampler, input.TexCoord);
    
    // ����
    // ���������̃x�N�g�����m�Ȃ琳�̒l
    // ���������̃x�N�g�����m�Ȃ�0
    // ���Ε����̃x�N�g�����m�Ȃ畉�̒l
    
    // �I�u�W�F�N�g�̖@���ƃ��C�g�̃x�N�g��������ς��v�Z
    // ���̂܂܌v�Z����Ɩ��邭�Ȃ镔�������̒l�ɂȂ�̂ŁA
    //�u�����āv���C�g�̃x�N�g���𔽓]������
    float3 N = normalize(input.Normal);
    float3 L = normalize(-g_vLightDir.xyz);
    
    // �g�U��(diffuse)���̂̕\�ʂɒ��ڂ�������A���̂̐F��\��
    float diffuse = dot(N, L);

    // ���ς̒l����Ƀ����vø����̐F����������o��
    // �����ς̌��ʂ�-1�`1�Ȃ̂�0�`1�ɕϊ�����
    diffuse = diffuse * 0.5f + 0.5f;
    
    float4 toonColor = g_rampTexture.Sample(g_sampler, float2(diffuse, 0));
    
    Diff.rgb *= toonColor.rgb;

    return Diff;
}
