//=========================================================
// [PS_Assimp.hlsl]
//---------------------------------------------------------
// �X�V:2023/01/23 ���ƂŃV�F�[�_�[����Ă���&if���C�ɂȂ����̂ŉ��C
// �X�V:2023/01/25 �e�ǉ�
// �X�V:2023/02/05 �萔�Ǘ��̂��߁A hlsli�ǉ�
// �X�V:2023/02/06 ���O�ύX
//---------------------------------------------------------
// Assimp�p�s�N�Z���V�F�[�_
// g_Flag��AssimpModel�̓s���ł��̂܂�
//=========================================================

#include <modelCommon.hlsli>

//
// �s�N�Z���V�F�[�_
//
float4 main(PS_INPUT input) : SV_Target0
{
    ModelOutput model;
    model.Pos = input.Pos;
    model.Normal = input.Normal;
    model.Tex = input.Tex;
    model.PosForPS = input.PosForPS;
    float4 Diff = CalcMaterial(g_material[input.id].fAmbient, g_material[input.id].fDiffuse, g_material[input.id].fSpecular, g_material[input.id].fEmissive, model);
    
    // �e
    // ���s��������������ø�������A�������擾
    float sunDepth = g_depthTexture.Sample(g_sampler, GetSunUV(input.sunPos)).r;
    // �V���h�E�A�N�l(�}�b�n�o���h)�΍�
    // �`�悷��s�N�Z���̉��s���ƁAø����̉��s������v���Ă����ꍇ�A
    // ø����ɏ������܂ꂽ�f�[�^�Ƃ̍��ق��������ĉe���o���Ȃ�������
    // �e���o���Ă��܂��Bø����ɏ������܂ꂽ�����͎��ۂ͏��������A�ƒu�����Ƃŉ���ł���
    sunDepth += 0.005f; // �����͓K������

    // ���s���m�F
    // pixel�̕������Ȃ�1.0f�A�܂�e
    float shadowRes = IsNear(input.sunPos, sunDepth);
    
    // �V���h�E�}�b�v
    float2 shadowUV = GetSunUV(input.sunPos);
    float result = 1.0f;
    // 0�ȏ�
    result *= step(0.0f, shadowUV.x);
    result *= step(0.0f, shadowUV.y);
    // 1�ȉ�
    result *= step(shadowUV.x, 1.0f);
    result *= step(shadowUV.y, 1.0f);
    shadowRes *= result;
    // 0�Ȃ�F�ύX�Ȃ��A1�Ȃ�e���e�����f
    shadowRes = 1.0f - shadowRes;
    shadowRes = saturate(shadowRes + 0.3f);
    
    //--- �e���ʔ��f
    // �e�Ȃ�S�č�
    Diff.rgb *= shadowRes;
    
    // �e�Ȃ�Ε`��
    //Alpha = shadowRes;

    return Diff;
    
}
