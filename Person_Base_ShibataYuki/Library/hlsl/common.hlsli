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
    float4 fAmbient;    // ���F
    float4 fDiffuse;    // �g�U�F+�A���t�@
    float4 fSpecular;   // ���ʔ��ːF+���x
    float4 fEmissive;   // �����F
    float4 fFlags;      // �g�U�F�e�N�X�`���L��, �����F�e�N�X�`���L��, ���߃e�N�X�`���L��
};


//==========================================================
// �萔�o�b�t�@
//---------------------------------------------------------
// cb0 WVP �`�掞
// cb1 Camera&Light �`��O
// cb2 �}�e���A�� �`�掞
// cb3 �{�[�� ���f���`�掞
// cb4 �C���X�^���V���O�}�g���b�N�X �`�掞
// cb5 �C���X�^���V���O�}�e���A�� �`�掞
// cb6 ���C�g��� �`��O
//==========================================================

// VS
// meshVS�ADepthWriteVS�Ŏg�p
cbuffer WVP : register(b0)
{
    matrix g_world;
    matrix g_view;
    matrix g_proj;
    matrix g_mTexture; // �e�N�X�`���s��
};

// PS/VS model
// �O���[�o��
cbuffer global : register(b1)
{
    float4 g_vCameraPos;     // ���_���W(���[���h���)
    float4 g_vLightDir;      // ��������(���[���h���)
    float4 g_vLightAmbient;  // ����
    float4 g_vLightDiffuse;  // �g�U��
    float4 g_vLightSpecular; // ���ʔ��ˌ�
};

// PS model
// �}�e���A��
cbuffer global2 : register(b2)
{
    float4 g_Ambient;   // ���F
    float4 g_Diffuse;   // �g�U�F+�A���t�@
    float4 g_Specular;  // ���ʔ��ːF+���x
    float4 g_Emissive;  // �����F
    float4 g_Flags;     // �g�U�F�e�N�X�`���L��, �����F�e�N�X�`���L��, ���߃e�N�X�`���L��
};

// VS
// �{�[���̃|�[�Y�s��
cbuffer global_bones : register(b3)
{
    matrix g_BoneWorld[MAX_BONE_MATRIX];
};

// VS model
// ���[���h�s��z��
cbuffer InstancingGlobal : register(b4)
{
    InstancingData g_Instancing[MAX_WORLD_MATRIX];
}

// PS Mesh/Model
// �C���X�^���V���O�g�p���̊e���}�e���A��
cbuffer InstancingMaterial : register(b5)
{
    MaterialData g_material[MAX_WORLD_MATRIX];
};

// ���z�̈ʒu�ɂ����ׂɕ\�����邽�߂̍s��
// meshVS
cbuffer SunCamera : register(b6)
{
    float4x4 g_sunView;
    float4x4 g_sunProj;
};

// ���[�g
// PS�p
cbuffer Rate : register(b7)
{
    float4 g_rate;
}


//==========================================================
// �֐�
//==========================================================
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

//==========================================================
// ø���/�����
//==========================================================
Texture2D g_texture : register(t0);         // �e�N�X�`��
Texture2D g_texEmissive : register(t1);     // �����e�N�X�`��
Texture2D g_texTransparent : register(t2);  // ���߃e�N�X�`��
Texture2D g_texSpecular : register(t3);     // ���ʔ��˃e�N�X�`��
Texture2D g_texSunView : register(t4);      // �eø���
Texture2D g_rampTexture : register(t5);     // Toon�p

SamplerState g_sampler : register(s0);      // �T���v��
