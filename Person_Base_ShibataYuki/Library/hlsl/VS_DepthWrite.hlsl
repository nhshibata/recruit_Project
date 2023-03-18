//==========================================================
// [VS_DepthWrite]
//---------------------------------------------------------
// �쐬:2023/01/22
// �X�V:2023/01/26 instanceID�ǉ�
// �X�V:2023/02/05 �萔�Ǘ��̂��߁A hlsli�ǉ�
// �X�V:2023/02/06 ���O�ύX
//---------------------------------------------------------
// Z�l��������DepthWritePS�ɓn�����
// InstancingVertex(model,mesh)�ɂ��̌�n�����
//==========================================================

#include <Common.hlsli>

struct VS_IN_Depth
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


VS_OUT main(VS_IN_Depth vin)
{
    VS_OUT vout;
    
    //--- ���[�J�����W >���[���h���W > �r���[���W >�v���W�F�N�V�������W �ƕϊ���������
    
    //float4x4 mWorld = mul(world, g_World[vin.id]);
    
    // �C���X�^���V���O���Ƃ�۰�ٍ��W����X�N���[�����W��
    float4x4 mWorld = g_Instancing[vin.id].mWorld;
    vout.pos = float4(vin.pos, 1.0f);
    vout.pos = mul(vout.pos, mWorld);
    vout.pos = mul(vout.pos, g_sunView);
    vout.pos = mul(vout.pos, g_sunProj);
    
    // SV_POSITION�Ƃ͕ʂ��ް���n��
    vout.screenPos = vout.pos;
    
    return vout;
}