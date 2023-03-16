//=========================================================
// [VS_Assimp.hlsl]
//---------------------------------------------------------
// �X�V:2023/02/05 �萔�Ǘ��̂��߁A hlsli�ǉ�
// �X�V:2023/02/06 ���O�ύX
//---------------------------------------------------------
// Assimp�p���_�V�F�[�_
//=========================================================

#include "modelCommon.hlsli"

struct VS_OUTPUT
{
    float4 Pos      : SV_Position;
    float2 Tex      : TEXCOORD0;
    float3 Normal   : TEXCOORD1;
    float4 PosForPS : TEXCOORD2;
    float4 SunPos   : TEXCOORD3; // ���z���猩���ʒu
    //float4 Diffuse  : COLOR0;
    
    uint id : SV_InstanceID; // �C���X�^���XID
};


//
// ���_�V�F�[�_
//
VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    // ���f�����ʏ���
    //ModelOutput model = SkinModel(input, g_Instancing[input.id].mWorld, g_world, g_view, g_proj, g_mTexture);
    //output.Pos = model.Pos;
    //output.Normal = model.Normal;
    //output.Tex = model.Tex;
    //output.PosForPS = model.PosForPS;
    
    //SKIN vSkinned = SkinVert(input);
    //float4x4 mWorld = mul(g_world, g_Instancing[input.id].mWorld);
    //float4x4 VP = mul(g_view, g_proj);
    //output.Pos = mul(vSkinned.Pos, mul(mWorld, VP));
    //output.Tex = mul(float4(input.Tex, 0.0f, 1.0f), g_mTexture).xy;
    //output.Normal = mul(vSkinned.Norm, (float3x3) mWorld);
    //output.PosForPS = mul(vSkinned.Pos, mWorld).xyz;
    
    //// ���z�p���_�v�Z
    //float4 wPos = mul(float4(input.Pos, 1.0f), g_Instancing[input.id].mWorld);
    //wPos = mul(wPos, g_sunView);
    //wPos = mul(wPos, g_sunProj);
    //output.SunPos = wPos;
    
    //output.id = input.id;
    
    SKIN vSkinned = SkinVert(input);
    float4x4 mWorld = mul(g_world, g_Instancing[input.id].mWorld);
    float4x4 VP = mul(g_view, g_proj);
    output.Pos = mul(vSkinned.Pos, mul(mWorld, VP));
    output.Tex = mul(float4(input.Tex, 0.0f, 1.0f), g_mTexture).xy;
    output.Normal = mul(vSkinned.Norm, (float3x3) mWorld);
    output.PosForPS = mul(vSkinned.Pos, mWorld);
    
    // ���z�p���_�v�Z
    float4 wPos = float4(input.Pos, 1.0f);
    wPos = mul(wPos, g_Instancing[input.id].mWorld);
    //wPos = mul(wPos, g_world);
    wPos = mul(wPos, g_sunView);
    wPos = mul(wPos, g_sunProj);
    output.SunPos = wPos;
    
    
    //output.Diffuse = float4(0, 0, 0, 0);
    output.id = input.id;
    
    return output;
}
