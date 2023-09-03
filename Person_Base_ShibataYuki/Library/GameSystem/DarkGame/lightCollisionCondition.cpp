//==========================================================
// [lightCollisionCondition.cpp]
//----------------------------------------------------------
// 作成:2023/06/01
//----------------------------------------------------------
// ライトの範囲確認用
// 条件を確認し、判定を保持する
//==========================================================

//--- インクルード部
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/DarkGame/lightCollisionCondition.h>

using namespace DarkGame;

//==========================================================
// コンストラクタ
//==========================================================
CLightCollisionCondition::CLightCollisionCondition()
	:m_bHit(false)
{

}

//==========================================================
// コンストラクタ
//==========================================================
CLightCollisionCondition::CLightCollisionCondition(CGameObject::Ptr ptr)
	:CLightCollision(ptr), m_bHit(false)
{

}

//==========================================================
// デストラクタ
//==========================================================
CLightCollisionCondition::~CLightCollisionCondition()
{

}

//==========================================================
// 生成時処理
//==========================================================
void CLightCollisionCondition::Awake()
{
	CLightCollision::Awake();
}

//==========================================================
// 更新
//==========================================================
void CLightCollisionCondition::Update()
{

}

//==========================================================
// 情報のクリア
//==========================================================
void CLightCollisionCondition::Refresh()
{
	m_bHit = false;
}

//==========================================================
// 条件を満たしているか
//==========================================================
bool CLightCollisionCondition::IsRange(std::string spec)
{
	return m_bHit;
}

//==========================================================
// 条件を満たした場合はtrue
//==========================================================
bool CLightCollisionCondition::Check(const CLightCollision& other)
{
	// 対象がタグ条件と一致するか
	if (other.GetOwner()->GetTagPtr()->Compare(m_SpecName))
	{
		// 範囲判定
		bool hit = CLightCollision::Check(other);
		m_bHit |= hit;

		return hit;
	}

	return false;
}