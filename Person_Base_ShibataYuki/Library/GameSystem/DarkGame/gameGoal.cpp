//==========================================================
// [gameGoal.cpp]
//----------------------------------------------------------
// 作成:2023/05/19
// 更新:2023/05/21 cpp実装
// 
//==========================================================

//--- インクルード部
#include <GameSystem/DarkGame/gameGoal.h>
#include <GameSystem/Component/Collision/boxCollision.h>
#include <GameSystem/Component/Renderer/modelRenderer.h>

using namespace DarkGame;

//==========================================================
// コンストラクタ
//==========================================================
CGameGoal::CGameGoal()
{

}

//==========================================================
// コンストラクタ
//==========================================================
CGameGoal::CGameGoal(CGameObject::Ptr ptr)
	:CComponent(ptr), m_bGoal(false)
{

}

//==========================================================
// 生成時処理
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
// 更新
//==========================================================
void CGameGoal::Update()
{
	
}

//==========================================================
// 衝突時
//==========================================================
void DarkGame::CGameGoal::OnTriggerEnter(CGameObject* obj)
{
	if (obj->GetTagPtr()->Compare("Player"))
	{
		m_bGoal = true;
	}
}
