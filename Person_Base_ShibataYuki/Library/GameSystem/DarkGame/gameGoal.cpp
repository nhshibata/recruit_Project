//==========================================================
// [gameGoal.cpp]
//----------------------------------------------------------
// �쐬:2023/05/19
// �X�V:2023/05/21 cpp����
// 
//==========================================================

//--- �C���N���[�h��
#include <GameSystem/DarkGame/gameGoal.h>
#include <GameSystem/Component/Collision/boxCollision.h>
#include <GameSystem/Component/Renderer/modelRenderer.h>

using namespace DarkGame;

//==========================================================
// �R���X�g���N�^
//==========================================================
CGameGoal::CGameGoal()
{

}

//==========================================================
// �R���X�g���N�^
//==========================================================
CGameGoal::CGameGoal(CGameObject::Ptr ptr)
	:CComponent(ptr), m_bGoal(false)
{

}

//==========================================================
// ����������
//==========================================================
void CGameGoal::Awake()
{
	{
		auto box = AddComponent<CBoxCollision>();
		box->SetTrigger(false);
	}

	AddComponent<CModelRenderer>();
}

//==========================================================
// �X�V
//==========================================================
void CGameGoal::Update()
{
	
}

//==========================================================
// �Փˎ�
//==========================================================
void DarkGame::CGameGoal::OnTriggerEnter(CGameObject* obj)
{
	if (obj->GetTagPtr()->Compare("Player"))
	{
		m_bGoal = true;
	}
}
