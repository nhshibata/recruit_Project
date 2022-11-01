//=========================================================
// [vertex.h] 
// �쐬:2022/07/25
//---------------------------------------------------------
// �\���̒�`
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __VERTEX__H__
#define __VERTEX__H__

//--- �C���N���[�h��
#include <DirectXTex.h>

// ���_�t�H�[�}�b�g( ���_���W[2D] / ���ˌ� / �e�N�X�`�����W )
typedef struct {
	DirectX::XMFLOAT3 vtx;		// ���_���W
	DirectX::XMFLOAT4 diffuse;	// �g�U���ˌ�
	DirectX::XMFLOAT2 tex;		// �e�N�X�`�����W
} VERTEX_2D;

// ���_�t�H�[�}�b�g( ���_���W[3D] / �@���x�N�g�� / ���ˌ� / �e�N�X�`�����W )
typedef struct {
	DirectX::XMFLOAT3 vtx;		// ���_���W
	DirectX::XMFLOAT3 nor;		// �@���x�N�g��
	DirectX::XMFLOAT4 diffuse;	// �g�U���ˌ�
	DirectX::XMFLOAT2 tex;		// �e�N�X�`�����W
} VERTEX_3D;


#endif // !__VERTEX__H__
