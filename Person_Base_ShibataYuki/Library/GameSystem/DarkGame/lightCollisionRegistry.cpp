//==========================================================
// [lightCollisionRegistry.cpp]
//----------------------------------------------------------
// 作成:2023/06/01
//----------------------------------------------------------
// ライトの範囲確認用
// 種別なく判定したらキャッシュする
//==========================================================

//--- インクルード部
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/DarkGame/lightCollisionRegistry.h>

using namespace DarkGame;

//==========================================================
// コンストラクタ
//==========================================================
CLightCollisionRegistry::CLightCollisionRegistry()
{

}

//==========================================================
// コンストラクタ
//==========================================================
CLightCollisionRegistry::CLightCollisionRegistry(CGameObject::Ptr ptr)
	:CLightCollision(ptr)
{

}

//==========================================================
// デストラクタ
//==========================================================
CLightCollisionRegistry::~CLightCollisionRegistry()
{

}

//==========================================================
// 生成時処理
//==========================================================
void CLightCollisionRegistry::Awake()
{
	CLightCollision::Awake();
}

//==========================================================
// 更新
//==========================================================
void CLightCollisionRegistry::Update()
{

}

//==========================================================
// 情報のクリア
//==========================================================
void CLightCollisionRegistry::Refresh()
{
	m_aHitTag.clear();
}

//==========================================================
// 条件を満たしているか
//==========================================================
bool CLightCollisionRegistry::IsRange(std::string spec)
{
	// 一致するタグがあった
	for (const auto & tag : m_aHitTag)
	{
		if (tag == spec)
		{
			return true;
		}
	}
	return false;
}

//==========================================================
// 条件を満たした場合はtrue
//==========================================================
bool CLightCollisionRegistry::Check(const CLightCollision& other)
{
	// 判定
	bool hit = CLightCollision::Check(other);
	if (hit)
	{
		// タグを記録
		m_aHitTag.push_back(other.GetOwner()->GetTag());
	}

	return hit;
}