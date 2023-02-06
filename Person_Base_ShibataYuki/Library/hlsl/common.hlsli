//=========================================================
// [common.hlsli]
//---------------------------------------------------------
// �X�V:2023/02/05 �萔�Ǘ��̂��߁A hlsli�ǉ�
//---------------------------------------------------------
// �萔�A�\���̊Ǘ�
// �Ȃ�ׂ�PS�AVS�ǂ��Ŏg�����R�����g
//=========================================================

//--- �萔��`
#define MAX_BONE_MATRIX	    64
#define MAX_WORLD_MATRIX    400
#define IF_SUB(x,y,z)       (x + ((x * y - x) * z))

//--- �\���̒�`
// �s��
struct InstancingData
{
    matrix mWorld;
};

// �}�e���A��
struct MaterialData
{
    float4 fAmbient; // ���F
    float4 fDiffuse; // �g�U�F+�A���t�@
    float4 fSpecular; // ���ʔ��ːF+���x
    float4 fEmissive; // �����F
    float4 fFlags; // �g�U�F�e�N�X�`���L��, �����F�e�N�X�`���L��, ���߃e�N�X�`���L��
};

// meshVS
// �O���[�o��
cbuffer global : register(b0)
{
    matrix g_mVP; // ���[���h�~�r���[�~�ˉe�s��
    matrix g_mWorld; // ���[���h�s��
    matrix g_mTexture;
};

// model�pPS/VS
// �O���[�o��
cbuffer global : register(b0)
{
    matrix g_mtxVP;         // �r���[�~�ˉe�s��
    matrix g_mtxWorld;      // ���[���h�s��
    matrix g_mtxTexture;    // �e�N�X�`���s��
    float4 g_vCameraPos;    // ���_���W(���[���h���)
    float4 g_vLightDir;     // ��������(���[���h���)
    float4 g_vLightAmbient; // ����
    float4 g_vLightDiffuse; // �g�U��
    float4 g_vLightSpecular; // ���ʔ��ˌ�
};

// model�pPS
// �}�e���A��
cbuffer global2 : register(b1)
{
    float4 g_Ambient;   // ���F
    float4 g_Diffuse;   // �g�U�F+�A���t�@
    float4 g_Specular;  // ���ʔ��ːF+���x
    float4 g_Emissive;  // �����F
    float4 g_Flags;     // �g�U�F�e�N�X�`���L��, �����F�e�N�X�`���L��, ���߃e�N�X�`���L��
};

// VS
// �{�[���̃|�[�Y�s��
cbuffer global_bones : register(b2)
{
    matrix g_BoneWorld[MAX_BONE_MATRIX];
};

// model VS
// ���[���h�s��z��
cbuffer InstancingGlobal : register(b3)
{
    InstancingData g_Instancing[MAX_WORLD_MATRIX];
}

// ���z�̈ʒu�ɂ����ׂɕ\�����邽�߂̍s��
// meshVS
cbuffer SunCamera : register(b4)
{
    float4x4 sunView;
    float4x4 sunProj;
};

// VS
// DepthWriteVS�Ŏg�p
cbuffer WVP : register(b5)
{
    float4x4 world;
    float4x4 view;
    float4x4 proj;
};

// PS Mesh/Model
// �C���X�^���V���O�g�p���̊e���}�e���A��
cbuffer InstancingMaterial : register(b6)
{
    MaterialData g_material[MAX_WORLD_MATRIX];
};
