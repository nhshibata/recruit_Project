//=========================================================
// [PS_AssimpToon]
//---------------------------------------------------------
// �쐬:2023/02/09
//---------------------------------------------------------
// �l�K
//=========================================================

#include "modelCommon.hlsli"

// �p�����[�^
struct PS_INPUT
{
    float4 Pos : SV_Position;
    float2 Tex : TEXCOORD0;
    float3 Normal : TEXCOORD1;
    float3 PosForPS : TEXCOORD2;
    float4 sunPos : TEXCOORD3; // ���z���猩���ʒu
    
    uint id : SV_InstanceID; // �C���X�^���XID
};

float4 main(PS_INPUT input) : SV_Target0
{
    ModelOutput model;
    model.Pos = input.Pos;
    model.Normal = input.Normal;
    model.Tex = input.Tex;
    model.PosForPS = input.PosForPS;
    float4 Diff = CalcMaterial(g_material[input.id].fAmbient, g_material[input.id].fDiffuse, g_material[input.id].fSpecular, g_material[input.id].fEmissive, model);
    
    float4 color = Diff;
    float3 negaColor;
    negaColor.r = 1.0f - color.r;
    negaColor.g = 1.0f - color.g;
    negaColor.b = 1.0f - color.b;
    Diff.rgb = lerp(color.rgb, negaColor, g_rate.r);
    
    return Diff;
}