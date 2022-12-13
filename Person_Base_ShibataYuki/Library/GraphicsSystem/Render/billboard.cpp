//=========================================================
// [billboard.h]
// �쐬: 2022/07/04
//=========================================================

#include <GraphicsSystem/Render/billboard.h>
#include <GraphicsSystem/Texture/spriteAnimation.h>
#include <GraphicsSystem/DirectX/DXDevice.h>
#include <GameSystem/Component/Camera/camera.h>
#include <GameSystem/Component/Light/directionalLight.h>

using namespace MySpace::System;
using namespace MySpace::Game;
using namespace MySpace::Graphics;


CBillboard::CBillboard()
{
}
CBillboard::~CBillboard()
{

}
HRESULT CBillboard::Init()
{
	// �ꎞ�I�Ȓ��_�z��𐶐�
	//nNumVertex = 4;
	VERTEX_3D* pVertexWk = new VERTEX_3D[4];

	// ���_���W�̐ݒ�
	pVertexWk[0].vtx = XMFLOAT3(-0.5f, 0.5f, 0.0f);
	pVertexWk[1].vtx = XMFLOAT3(0.5f, 0.5f, 0.0f);
	pVertexWk[2].vtx = XMFLOAT3(-0.5f, -0.5f, 0.0f);
	pVertexWk[3].vtx = XMFLOAT3(0.5f, -0.5f, 0.0f);
	// ���ˌ��̐ݒ�
	pVertexWk[0].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[1].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[2].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[3].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	// �@���̐ݒ�
	pVertexWk[0].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[1].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[2].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[3].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	// �e�N�X�`�����W�̐ݒ�
	pVertexWk[0].tex = XMFLOAT2(0.0f, 0.0f);
	pVertexWk[1].tex = XMFLOAT2(1.0f, 0.0f);
	pVertexWk[2].tex = XMFLOAT2(0.0f, 1.0f);
	pVertexWk[3].tex = XMFLOAT2(1.0f, 1.0f);

	// �ꎞ�I�ȃC���f�b�N�X�z��𐶐�
	//nNumIndex = 4;
	int* pIndexWk = new int[4];
	// �C���f�b�N�X�z��̒��g�𖄂߂�
	pIndexWk[0] = 0;
	pIndexWk[1] = 1;
	pIndexWk[2] = 2;
	pIndexWk[3] = 3;

	HRESULT hr = CMesh::Init(pVertexWk, 4, pIndexWk, 4);

	// �ꎞ�I�Ȓ��_�z��/�C���f�b�N�X�z������
	delete[] pIndexWk;
	delete[] pVertexWk;
	return hr;
}

void CBillboard::Update(Vector3 pos, Vector2 size)
{
	// �r���[�}�g���b�N�X���擾
	XMFLOAT4X4& mtxView = CCamera::GetMain()->GetViewMatrix();
	XMFLOAT4X4 mtx;
	//XMMATRIX mtxWorld;
	//// ���[���h�}�g���b�N�X�̏�����
	//mtxWorld = XMMatrixIdentity();

	// ��]�𔽉f
	mtx._11 = mtxView._11 * size.x;
	mtx._12 = mtxView._21 * size.x;
	mtx._13 = mtxView._31 * size.x;
	mtx._14 = 0.0f;
	mtx._21 = mtxView._12 * size.y;
	mtx._22 = mtxView._22 * size.y;
	mtx._23 = mtxView._32 * size.y;
	mtx._24 = 0.0f;
	mtx._31 = mtxView._13;
	mtx._32 = mtxView._23;
	mtx._33 = mtxView._33;
	mtx._34 = 0.0f;

	// �ʒu�𔽉f
	mtx._41 = pos.x;
	mtx._42 = pos.y;
	mtx._43 = pos.z;
	mtx._44 = 1.0f;

	SetWorld(&mtx);

	// �m�F
	//GetMaterial()->m_Diffuse.x += 0.01f;
	//if (GetMaterial()->m_Diffuse.x > 1)
	//	GetMaterial()->m_Diffuse.x = 0;
}
//
//void CBillboard::Draw(ID3D11ShaderResourceView* pTex, CSpriteAnimation* pSprite)
//{
//	
//}