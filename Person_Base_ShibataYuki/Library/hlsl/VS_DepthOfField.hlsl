//==========================================================
// [DepthOfField.hlsl]
//---------------------------------------------------------
// ��ʊE�[�x
//==========================================================

#include <Common.hlsli>

// ���_�V�F�[�_�[�ւ̓���
struct VSIn
{
    float4 pos : POSITION; // ���f���̒��_���W
    float3 normal : NORMAL; // �@��
    float3 tangent : TANGENT;
    float3 biNormal : BINORMAL;
    float2 uv : TEXCOORD0; // UV���W
};

// �s�N�Z���V�F�[�_�[�ւ̓���
struct PSIn
{
    float4 pos : SV_POSITION; // �X�N���[����Ԃł̃s�N�Z���̍��W
    float3 normal : NORMAL; // �@��
    float3 tangent : TANGENT; // �ڃx�N�g��
    float3 biNormal : BINORMAL; // �]�@���x�N�g��
    float2 uv : TEXCOORD0; // UV���W
    float3 worldPos : TEXCOORD1; // ���[���h��Ԃł̃s�N�Z���̍��W

    // step-7 �J������Ԃł�Z�l���L�^����ϐ���ǉ�
    float3 depthInView : TEXCOORD2; // �J������Ԃł�Z�l
};


PSIn main(VSIn vin)
{
    PSIn pout;

    //pout.pos = mul(g_world, vin.pos);
    //pout.worldPos = pout.pos;
    //pout.worldPos = mul(g_view, pout.pos);
    
    //pout.pos = mul(g_proj, pout.pos);
    
    //// 4x3�ɕϊ�
    //float4x3 mWorld = (float4x3)g_world;
    //pout.normal = normalize(mul(mWorld, vin.normal));
    //pout.tangent = normalize(mul(mWorld, vin.tangent));
    //pout.biNormal = normalize(mul(mWorld, vin.biNormal));
    //pout.uv = vin.uv;
    
	return pout;
}