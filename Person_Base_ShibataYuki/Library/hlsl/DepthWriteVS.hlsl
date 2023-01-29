//==========================================================
// [DepthWriteVS]
//---------------------------------------------------------
// �쐬:2023/01/22
// �X�V:2023/01/26 instanceID�ǉ�
//---------------------------------------------------------
// Z�l��������DepthWritePS�ɓn�����
// InstancingVertex(model,mesh)�ɂ��̌�n�����
//==========================================================

#define MAX_WORLD_MATRIX 1024

struct VS_IN
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    
    uint id : SV_InstanceID; // �C���X�^���XID
};

struct VS_OUT
{
    float4 pos : SV_POSITION;
    float4 screenPos : TEXCOORD0;
};

// ���[���h�s��z��
cbuffer global_instance : register(b3)
{
    matrix g_World[MAX_WORLD_MATRIX];
};

cbuffer WVP : register(b5)
{
    float4x4 world;
    float4x4 view;
    float4x4 proj;
};

VS_OUT main(VS_IN vin)
{
    VS_OUT vout;
    
    //--- ���[�J�����W >���[���h���W > �r���[���W >�v���W�F�N�V�������W �ƕϊ���������
    
    //float4x4 mWorld = mul(world, g_World[vin.id]);
    
    // �C���X�^���V���O���Ƃ�۰�ٍ��W����X�N���[�����W��
    float4x4 mWorld = g_World[vin.id];
    vout.pos = float4(vin.pos, 1.0f);
    vout.pos = mul(vout.pos, mWorld);
    vout.pos = mul(vout.pos, view);
    vout.pos = mul(vout.pos, proj);
    
    // SV_POSITION�Ƃ͕ʂ��ް���n��
    vout.screenPos = vout.pos;
    
    return vout;
}