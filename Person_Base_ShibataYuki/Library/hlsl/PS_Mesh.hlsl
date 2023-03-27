//=========================================================
// [PS_Mesh]
//---------------------------------------------------------
// �X�V:2023/01/25 �e�ǉ�
// �X�V:2023/02/06 ���O�ύX
//---------------------------------------------------------
// mesh�p
//=========================================================

#include <meshCommon.hlsli>

//Texture2D    g_texture    : register(t0);	// �e�N�X�`��
//Texture2D    g_texSunView : register(t1);   // ���zø���
//SamplerState g_sampler    : register(s0);	// �T���v��

float4 main(PS_INPUT input) : SV_Target0
{
    // mesh�̋��ʊ֐�
    MeshOutput vin;
    vin.Position = input.Pos;
    vin.Pos4PS = input.PosForPS;
    vin.Diffuse = input.Diffuse;
    vin.TexCoord = input.Tex;
    vin.Normal = input.Normal;
    float4 Diff = CalcPSMaterial(vin, g_material[input.id].fAmbient, g_material[input.id].fDiffuse,
                    g_material[input.id].fSpecular, g_material[input.id].fEmissive, g_material[input.id].fFlags);

    // �e
    // ���s��������������ø�������A�������擾
    float sunDepth = g_texSunView.Sample(g_sampler, GetSunUV(input.sunPos)).r;
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
    //// 0�ȏ�
    //result *= step(0.0f, shadowUV.x);
    //result *= step(0.0f, shadowUV.y);
    //// 1�ȉ�
    //result *= step(shadowUV.x, 1.0f + 0.0001f);
    //result *= step(shadowUV.y, 1.0f + 0.0001f);
    result *= step(0.0f, shadowUV.x) * (1.0f - step(1.0f, shadowUV.x));
    result *= step(0.0f, shadowUV.y) * (1.0f - step(1.0f, shadowUV.y));
    shadowRes *= result;
    // 0�Ȃ�F�ύX�Ȃ��A1�Ȃ�e���e�����f
    shadowRes = 1.0f - shadowRes;
    shadowRes = saturate(shadowRes + 0.3f * shadowRes);
    
    //--- �e���ʔ��f
    // �e�Ȃ�S�č�
    Diff.rgb *= shadowRes;
    // �e�Ȃ�Ε`��
    //Alpha = shadowRes;
    
	return Diff;
}

