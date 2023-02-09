//=========================================================
// [VS_Mesh]
//---------------------------------------------------------
// �X�V:2023/02/05 �萔�Ǘ��̂��߁A hlsli�ǉ�
// �X�V:2023/02/06 ���O�ύX
//---------------------------------------------------------
// ���_�V�F�[�_�C���X�^���V���O�p
// �C���X�^���V���O�O��
//=========================================================

#include "meshCommon.hlsli"


struct VS_OUTPUT {
	float4	Position	: SV_Position;
	float3	Pos4PS		: TEXCOORD0;
	float3	Normal		: TEXCOORD1;
	float2	TexCoord	: TEXCOORD2;
	float4	Diffuse		: COLOR0;
    float4  SunPos      : TEXCOORD3; // ���z���猩���ʒu
    
    uint id : SV_InstanceID; // �C���X�^���XID
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
    uint id = input.id % MAX_WORLD_MATRIX;

    MeshOutput work = CalcMesh(input, g_Instancing[id].mWorld, g_view, g_proj);
    output.Position = work.Position;
    output.Pos4PS = work.Pos4PS;
    output.Normal = work.Normal;
    output.TexCoord = work.TexCoord;
    output.Diffuse = work.Diffuse;

     // ���z�p���_�v�Z
    float4 sunPos = float4(input.Position.xyz, 1.0f);
    sunPos = mul(sunPos, g_Instancing[id].mWorld);
    sunPos = mul(sunPos, g_sunView);
    sunPos = mul(sunPos, g_sunProj);
    output.SunPos = sunPos;
    
    output.id = id;
    
	return output;
}
