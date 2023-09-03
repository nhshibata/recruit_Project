//==========================================================
// [goalMeterUI.h]
//----------------------------------------------------------
// �쐬:2023/05/21
// 
//==========================================================

//--- �C���N���[�h��
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/DarkGame/goalMeterUI.h>
#include <GameSystem/DarkGame/gameGoal.h>

using namespace DarkGame;

//==========================================================
// �R���X�g���N�^
//==========================================================
CGameMeterUI::CGameMeterUI()
{

}

//==========================================================
// �R���X�g���N�^
//==========================================================
CGameMeterUI::CGameMeterUI(CGameObject::Ptr ptr)
	:CComponent(ptr)
{

}

//==========================================================
// ������
//==========================================================
void CGameMeterUI::Awake()
{
	m_pText = GetOwner()->AddComponent<CTextRenderer>();
}

//==========================================================
// ������
//==========================================================
void CGameMeterUI::Init()
{

	{
		const auto& obj = CGameObject::FindGameObjectWithTag(CDarkPlayer::TAG);
		m_pDarkPlayer = obj.lock()->GetComponent<CDarkPlayer>();
	}
	
	{
		const auto& obj = CGameObject::FindGameObjectWithTag(CGameGoal::TAG);
		m_pGoalTransform = obj.lock()->GetTransform(1);
	}

}

//==========================================================
// �X�V
//==========================================================
void CGameMeterUI::Update()
{
	//--- �ʒu�̕\���X�V


}