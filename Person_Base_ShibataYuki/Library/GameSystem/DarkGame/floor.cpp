//==========================================================
// [floor.cpp]
//----------------------------------------------------------
// �쐬:2023/05/19 
// 
//==========================================================

//--- �C���N���[�h��
#include <GameSystem/DarkGame/floor.h>
#include <GameSystem/Component/Collision/boxCollision.h>
#include <GameSystem/Component/Renderer/boxRenderer.h>

using namespace DarkGame;

//--- �����o�֐�

//==========================================================
// �R���X�g���N�^
//==========================================================
CFloor::CFloor()
{

}

//==========================================================
// �R���X�g���N�^
//==========================================================
CFloor::CFloor(CGameObject::Ptr ptr)
	:CComponent(ptr)
{
	
}

//==========================================================
// ����������
//==========================================================
void CFloor::Awake()
{
	CTag::CreateTag(CFloor::TAG);
	GetOwner()->SetObjTag(CFloor::TAG);

	{
		auto box = AddComponent<CBoxCollision>();
		box->SetTrigger(false);
	}

	AddComponent<CBoxRenderer>();
}

//==========================================================
// �X�V
//==========================================================
void CFloor::Update()
{

}