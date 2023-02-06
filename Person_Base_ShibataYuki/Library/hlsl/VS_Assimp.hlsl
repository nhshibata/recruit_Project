//=========================================================
// [VS_Assimp.hlsl]
//---------------------------------------------------------
// �X�V:2023/02/05 �萔�Ǘ��̂��߁A hlsli�ǉ�
// �X�V:2023/02/06 ���O�ύX
//---------------------------------------------------------
// Assimp�p���_�V�F�[�_
//=========================================================

#include "common.hlsli"

// �p�����[�^
struct VS_INPUT
{
    float3 Pos : POSITION;
    float3 Normal : NORMAL;
    float2 Tex : TEXCOORD0;
    uint4 Bone : BONE_INDEX; // �{�[���̃C���f�b�N�X
    float4 Weight : BONE_WEIGHT; // �{�[���̏d��
    
    uint id : SV_InstanceID; // �C���X�^���XID
};

struct VS_OUTPUT
{
    float4 Pos : SV_Position;
    float2 Tex : TEXCOORD0;
    float3 Normal : TEXCOORD1;
    float3 PosForPS : TEXCOORD2;
    float4 SunPos : TEXCOORD3; // ���z���猩���ʒu
    
    uint id : SV_InstanceID; // �C���X�^���XID
};

// �X�L�j���O��̒��_�E�@��
struct SKIN
{
    float4 Pos;
    float3 Norm;
};

// ���_���X�L�j���O (�{�[���ɂ��ړ�)
SKIN SkinVert(VS_INPUT input)
{
    SKIN output;
    float4 Pos = float4(input.Pos, 1.0f);
    float3 Norm = input.Normal;
    if (input.Weight.x == 0.0f)
    {
		// �{�[������
        output.Pos = Pos;
        output.Norm = Norm;
    }
    else
    {
		// �{�[��0
        uint uBone = input.Bone.x;
        float fWeight = input.Weight.x;
        matrix m = g_BoneWorld[uBone];
        output.Pos = fWeight * mul(Pos, m);
        output.Norm = fWeight * mul(Norm, (float3x3) m);
		// �{�[��1
        uBone = input.Bone.y;
        fWeight = input.Weight.y;
        m = g_BoneWorld[uBone];
        output.Pos += fWeight * mul(Pos, m);
        output.Norm += fWeight * mul(Norm, (float3x3) m);
		// �{�[��2
        uBone = input.Bone.z;
        fWeight = input.Weight.z;
        m = g_BoneWorld[uBone];
        output.Pos += fWeight * mul(Pos, m);
        output.Norm += fWeight * mul(Norm, (float3x3) m);
		// �{�[��3
        uBone = input.Bone.w;
        fWeight = input.Weight.w;
        m = g_BoneWorld[uBone];
        output.Pos += fWeight * mul(Pos, m);
        output.Norm += fWeight * mul(Norm, (float3x3) m);
    }
    return output;
}

//
// ���_�V�F�[�_
//
VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    SKIN vSkinned = SkinVert(input);
    float4x4 mWorld = mul(g_mtxWorld, g_Instancing[input.id].mWorld);
    output.Pos = mul(vSkinned.Pos, mul(mWorld, g_mtxVP));
    output.Tex = mul(float4(input.Tex, 0.0f, 1.0f), g_mtxTexture).xy;
    output.Normal = mul(vSkinned.Norm, (float3x3) mWorld);
    output.PosForPS = mul(vSkinned.Pos, mWorld).xyz;
    
    // ���z�p���_�v�Z
    //float4 wPos = mul(float4(input.Pos, 1.0f), mWorld);
    float4 wPos = mul(float4(input.Pos, 1.0f), g_Instancing[input.id].mWorld);
    wPos = mul(wPos, sunView);
    wPos = mul(wPos, sunProj);
    output.SunPos = wPos;
    
    output.id = input.id;
    
    return output;
}
