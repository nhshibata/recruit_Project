//==========================================================
// [goalMeterUI.h]
//----------------------------------------------------------
// 作成:2023/05/21
// 
//==========================================================

//--- インクルード部
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/DarkGame/goalMeterUI.h>
#include <GameSystem/DarkGame/gameGoal.h>

using namespace DarkGame;

//==========================================================
// コンストラクタ
//==========================================================
CGameMeterUI::CGameMeterUI()
{

}

//==========================================================
// コンストラクタ
//==========================================================
CGameMeterUI::CGameMeterUI(CGameObject::Ptr ptr)
	:CComponent(ptr)
{

}

//==========================================================
// 生成時
//==========================================================
void CGameMeterUI::Awake()
{
	m_pText = GetOwner()->AddComponent<CTextRenderer>();
}

//==========================================================
// 生成時
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
// 更新
//==========================================================
void CGameMeterUI::Update()
{
	//--- 位置の表示更新


}