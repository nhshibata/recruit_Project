//==========================================================
// [floor.cpp]
//----------------------------------------------------------
// 作成:2023/05/19 
// 
//==========================================================

//--- インクルード部
#include <GameSystem/DarkGame/floor.h>
#include <GameSystem/Component/Collision/boxCollision.h>
#include <GameSystem/Component/Renderer/boxRenderer.h>

using namespace DarkGame;

//--- メンバ関数

//==========================================================
// コンストラクタ
//==========================================================
CFloor::CFloor()
{

}

//==========================================================
// コンストラクタ
//==========================================================
CFloor::CFloor(CGameObject::Ptr ptr)
	:CComponent(ptr)
{
	
}

//==========================================================
// 生成時処理
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
// 更新
//==========================================================
void CFloor::Update()
{

}