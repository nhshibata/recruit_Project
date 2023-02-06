//=========================================================
// Assimp�p�s�N�Z���V�F�[�_ (AssimpPixel.hlsl)
//---------------------------------------------------------
// �X�V:2023/02/05 �萔�Ǘ��̂��߁Ahlsli�ǉ�
//=========================================================

#include "common.hlsli"
//#include <common.hlsli>

// �p�����[�^
struct VS_INPUT {
	float4	Pos			: SV_Position;
	float2	Tex			: TEXCOORD0;
	float3	Normal		: TEXCOORD1;
	float3	PosForPS	: TEXCOORD2;
    
};

Texture2D    g_texture			: register(t0);	// �e�N�X�`��
Texture2D    g_texEmissive		: register(t1);	// �����e�N�X�`��
Texture2D    g_texTransparent	: register(t2);	// ���߃e�N�X�`��
Texture2D    g_texSpecular		: register(t3);	// ���ʔ��˃e�N�X�`��
SamplerState g_sampler			: register(s0);	// �T���v��

//
// �s�N�Z���V�F�[�_
//
float4 main(VS_INPUT input) : SV_Target0
{
	float3 Diff = g_Diffuse.rgb;
	float Alpha = g_Diffuse.a;
    float3 Ambi = g_Ambient.rgb;
     // �g�U�F�e�N�X�`���L��
    {
        float x = any(g_Flags.x);
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
    clip(Alpha - 1.0f); // ���S�����Ȃ�`�悵�Ȃ�
    
	// ���߃e�N�X�`���L��
    float3 Spec = g_Specular.rgb;
    {
        float4 TexSpec = g_texSpecular.Sample(g_sampler, input.Tex);
        int w = any(g_Flags.w);
        Spec = IF_SUB(Spec, TexSpec.rgb, w);
    }

  {
        int nLight = any(g_vLightDir);
        float3 L = normalize(-g_vLightDir.xyz);                  // �����ւ̃x�N�g��
        float3 N = normalize(input.Normal);                      // �@���x�N�g��
        float3 V = normalize(g_vCameraPos.xyz - input.PosForPS); // ���_�ւ̃x�N�g��
        float3 H = normalize(L + V);                             // �n�[�t�x�N�g��
        Diff = nLight ? g_vLightAmbient.rgb * g_Ambient.rgb +
			g_vLightDiffuse.rgb * Diff * saturate(dot(L, N)) : Diff; // �g�U�F + ���F
        Spec = nLight ? Spec * g_vLightSpecular.rgb *
			pow(saturate(dot(N, H)), g_Specular.a) : Spec; // ���ʔ��ːF
        Diff += Spec;
    }

	float3 Emis = g_Emissive.rgb;
	{   // �e�N�X�`���L��
        int y = any(g_Flags.y);
		float4 TexEmis = g_texEmissive.Sample(g_sampler, input.Tex);
        Emis = IF_SUB(Emis, TexEmis.rgb * TexEmis.a, y);
    }
	Diff += Emis;

	return float4(Diff, Alpha);
}
