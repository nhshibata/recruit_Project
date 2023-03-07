//=========================================================
// [meshCommon]
//---------------------------------------------------------
//
//
//=========================================================

#include "common.hlsli"

// �p�����[�^
struct VS_INPUT
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
    float4 Diffuse : COLOR0;
    
    uint id : SV_InstanceID; // �C���X�^���XID
};

struct MeshOutput
{
    float4 Position : SV_Position;
    float4 Pos4PS   : TEXCOORD0;
    float3 Normal   : TEXCOORD1;
    float2 TexCoord : TEXCOORD2;
    float4 Diffuse  : COLOR0;
};

MeshOutput CalcMesh(VS_INPUT input, matrix world, matrix view, matrix proj)
{
    MeshOutput output;
    float4 wPos = mul(float4(input.Position, 1.0f), world);
    float4x4 VP = mul(view, proj);
    output.Position = mul(wPos, VP);
    output.Pos4PS = wPos;
    output.Normal = mul(float4(input.Normal, 0.0f), world).xyz;
    output.TexCoord = mul(float4(input.TexCoord, 0.0f, 1.0f), g_mTexture).xy;
    output.Diffuse = input.Diffuse;
    return output;
}

/*
 float4 wPos = mul(float4(input.Position, 1.0f), g_Instancing[id].mWorld);
    float4x4 VP = mul(g_view, g_proj);
    output.Position = mul(wPos, VP);
    output.Pos4PS = wPos.xyz;
    output.Normal = mul(float4(input.Normal, 0.0f), g_Instancing[id].mWorld).xyz;
    output.TexCoord = mul(float4(input.TexCoord, 0.0f, 1.0f), g_mTexture).xy;
	output.Diffuse = input.Diffuse;
*/

float4 CalcPSMaterial(MeshOutput input, float4 ambi, float4 diff, float4 spec, float4 emi, float4 flag)
{
    float3 Diff = input.Diffuse.rgb * diff.rgb;
    float3 Ambi = input.Diffuse.rgb * ambi.rgb;
    float Alpha = input.Diffuse.a * diff.a;
    
    //--- �A���r�G���g�̃��l0���� == ø�������
    //float fTex = step(0.0f, g_vKa.a);
    float fTex = any(ambi.a);
    //if (0.0f < g_vKa.a)
    {
		// �e�N�X�`���L
        float4 vTd = g_texture.Sample(g_sampler, input.TexCoord);
        Diff = IF_SUB(Diff, vTd.rgb, fTex);
        Ambi = IF_SUB(Ambi, vTd.rgb, fTex);
        Alpha = IF_SUB(Alpha, vTd.a, fTex);
    }

    //if (Alpha <= 0.0f)
       // discard;
    clip(Alpha - 0.001f);

    // ligth�����ꂩ��0�ȊO�A���Aflg.r��0�̎��K�p
    if (any(g_vLightDir) & !any(flag.r))
    {
		// �����L��
        float3 L = normalize(-g_vLightDir.xyz); // �����ւ̃x�N�g��
        float3 N = normalize(input.Normal); // �@���x�N�g��
        float3 V = normalize(g_vCameraPos.xyz - input.Pos4PS.xyz); // ���_�ւ̃x�N�g��
        float3 H = normalize(L + V); // �n�[�t�x�N�g��
        float4 Spec = spec;
        Diff = g_vLightAmbient.rgb * Ambi + g_vLightDiffuse.rgb *
			Diff * saturate(dot(L, N)); // �g�U�F + ���F
        float3 newSpec = g_vLightSpecular.rgb * Spec.rgb *
			pow(saturate(dot(N, H)), Spec.a); // ���ʔ��ːF
        Diff += newSpec;
    }
    
    Diff += emi.rgb;
    return float4(Diff, Alpha);
}