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

#include "common.hlsli"

// �p�����[�^
struct VS_INPUT {
	float4	Pos			: SV_Position;
	float2	Tex			: TEXCOORD0;
	float3	Normal		: TEXCOORD1;
	float3	PosForPS	: TEXCOORD2;
    float4  sunPos      : TEXCOORD3; // ���z���猩���ʒu
    
    uint id : SV_InstanceID; // �C���X�^���XID
};

Texture2D    g_texture			: register(t0);	// �e�N�X�`��
Texture2D    g_texEmissive		: register(t1);	// �����e�N�X�`��
Texture2D    g_texTransparent	: register(t2);	// ���߃e�N�X�`��
Texture2D    g_texSpecular		: register(t3);	// ���ʔ��˃e�N�X�`��
Texture2D    g_texSunView       : register(t4); // ���zø���
SamplerState g_sampler			: register(s0);	// �T���v��

float2 GetSunUV(float4 sunPos)
{
    //--- �X�N���[�����W�ɍ��킹��
    // ���W��w�Ŋ���
    float2 uv = sunPos.xy / sunPos.w;
     // ��ʂ̍��W-1�`1��ø����̍��W0�`1�֕ϊ�
    uv = uv * 0.5f + 0.5f;
    // ��ʂ�Y���W�͉������Ɍ������đ����邪�Aø�����Y���W�͏ォ�牺�Ɍ������đ�����̂Ŕ��]
    uv.y = 1.0f - uv.y;
    return uv;
}

// ���s���m�F(���������pixel�̉��s������Ȃ�1)
// 1��0���Ԃ�
float IsNear(float4 pixelPos, float sunDepth)
{
    float ret = 0.0f;
    float pixelDepth = pixelPos.z / pixelPos.w;
    ret = step(sunDepth, pixelDepth);
    return ret;
}

//
// �s�N�Z���V�F�[�_
//
float4 main(VS_INPUT input) : SV_Target0
{
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
    
    
    float3 Diff = g_Diffuse.rgb * g_material[input.id].fDiffuse.rgb;
    float3 Ambi = g_Ambient.rgb * g_material[input.id].fAmbient.rgb;
    float Alpha = g_Diffuse.a * g_material[input.id].fDiffuse.a;
     // �g�U�F�e�N�X�`���L��
    {
        float x = any(g_Flags.x); // not zero == 1
        float4 TexDiff = g_texture.Sample(g_sampler, input.Tex);
        Diff =  IF_SUB(Diff, TexDiff.rgb, x);
        Ambi =  IF_SUB(Ambi, TexDiff.rgb, x);
        Alpha = IF_SUB(Alpha, TexDiff.a, x);
    }
    
    // �����F�e�N�X�`���L��
    {
        int z = any(g_Flags.z);
        float4 TexTran = g_texTransparent.Sample(g_sampler, input.Tex);
        Alpha = IF_SUB(Alpha, TexTran.r * TexTran.g * TexTran.b * TexTran.a, z);
    }
    clip(Alpha - 0.0001f); // ���S�����Ȃ�`�悵�Ȃ�
    
	// ���߃e�N�X�`���L��
    float3 Spec = g_Specular.rgb * g_material[input.id].fSpecular.rgb;
    {
        float4 TexSpec = g_texSpecular.Sample(g_sampler, input.Tex);
        int w = any(g_Flags.w);
        Spec = IF_SUB(Spec, TexSpec.rgb, w);
    }
    
    //--- ��
    float3 workDiff = Diff;
    {
        float3 L = normalize(-g_vLightDir.xyz);                  // �����ւ̃x�N�g��
        float3 N = normalize(input.Normal);                      // �@���x�N�g��
        float3 V = normalize(g_vCameraPos.xyz - input.PosForPS); // ���_�ւ̃x�N�g��
        float3 H = normalize(L + V);                             // �n�[�t�x�N�g��
        float3 mulAmbi = g_material[input.id].fAmbient.rgb * g_Ambient.rgb;
        float spec = g_Specular.a * g_material[input.id].fSpecular.a;
        workDiff = g_vLightAmbient.rgb * mulAmbi.rgb + g_vLightDiffuse.rgb * Diff * saturate(dot(L, N)); // �g�U�F + ���F
        Spec = Spec * g_vLightSpecular.rgb * pow(saturate(dot(N, H)), spec); // ���ʔ��ːF
        workDiff += Spec;
    }
    Diff = any(g_vLightDir) ? workDiff : Diff;

    float3 Emis = g_Emissive.rgb * g_material[input.id].fEmissive.rgb;
	{   // �e�N�X�`���L��
        int y = any(g_Flags.y);
		float4 TexEmis = g_texEmissive.Sample(g_sampler, input.Tex);
        Emis = IF_SUB(Emis, TexEmis.rgb * TexEmis.a, y);
    }
	Diff += Emis;
    
    //--- �e���ʔ��f
    // �e�Ȃ�S�č�
    Diff.rgb *= 1.0f - shadowRes;
    
    // �e�Ȃ�Ε`��
    //Alpha = shadowRes;

	return float4(Diff, Alpha);
}
