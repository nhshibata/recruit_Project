//==========================================================
// [gameStartPosition.cpp]
//----------------------------------------------------------
// 作成:2023/05/21
// 
//==========================================================

//--- インクルード部
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/DarkGame/gameStartPosition.h>
#include <GameSystem/DarkGame/darkPlayer.h>

using namespace DarkGame;

//==========================================================
// コンストラクタ
//==========================================================
CGameStartPosition::CGameStartPosition()
{

}

//==========================================================
// コンストラクタ
//==========================================================
CGameStartPosition::CGameStartPosition(CGameObject::Ptr ptr)
	:CComponent(ptr)
{

}

//==========================================================
// 生成時処理
//==========================================================
void CGameStartPosition::Awake()
{
	// タグの設定
	CTag::CreateTag(CGameStartPosition::TAG);
	GetOwner()->SetObjTag(CGameStartPosition::TAG);
}

//==========================================================
// 初期化処理
//==========================================================
void CGameStartPosition::Init()
{
	// プレイヤーの初期座標の取得
	const auto& obj = CGameObject::FindGameObjectWithTag(CDarkPlayer::TAG);
	m_vPosition = obj.lock()->GetTransform()->GetPos();
}

//==========================================================
// 更新
//==========================================================
void CGameStartPosition::Update()
{

}