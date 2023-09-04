//==========================================================
// [gameStartPosition.cpp]
//----------------------------------------------------------
// �쐬:2023/05/21
// 
//==========================================================

//--- �C���N���[�h��
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/DarkGame/gameStartPosition.h>
#include <GameSystem/DarkGame/darkPlayer.h>

using namespace DarkGame;

//==========================================================
// �R���X�g���N�^
//==========================================================
CGameStartPosition::CGameStartPosition()
{

}

//==========================================================
// �R���X�g���N�^
//==========================================================
CGameStartPosition::CGameStartPosition(CGameObject::Ptr ptr)
	:CComponent(ptr)
{

}

//==========================================================
// ����������
//==========================================================
void CGameStartPosition::Awake()
{
	// �^�O�̐ݒ�
	CTag::CreateTag(CGameStartPosition::TAG);
	GetOwner()->SetObjTag(CGameStartPosition::TAG);
}

//==========================================================
// ����������
//==========================================================
void CGameStartPosition::Init()
{
	// �v���C���[�̏������W�̎擾
	const auto& obj = CGameObject::FindGameObjectWithTag(CDarkPlayer::TAG);
	m_vPosition = obj.lock()->GetTransform()->GetPos();
}

//==========================================================
// �X�V
//==========================================================
void CGameStartPosition::Update()
{

}