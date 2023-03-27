//=====================================================
// [collisionSystem.cpp]
// 作成:2022/11/9
// 更新:2023/03/22 変な処理になってたので、スッキリ修正
//-----------------------------------------------------
//
//=====================================================

//--- インクルード部
#include <GameSystem/Manager/collisionSystem.h>
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Component/Collision/collision.h>

using namespace MySpace::Game;

//==========================================================
// コンストラクタ
//==========================================================
CCollisionSystem::CCollisionSystem()
{
}

//==========================================================
// デストラクタ
//==========================================================
CCollisionSystem::~CCollisionSystem()
{
	m_aIntMap.clear();
}

//==========================================================
// Collisionｸﾗｽ同士の当たり判定実行
// REVIEW: 改善の余地あり
//==========================================================
void CCollisionSystem::CollisionCheck()
{
	if (m_aIntMap.size() == 0)
		return;
	
	//--- 当たり判定を行うｺﾝﾎﾟｰﾈﾝﾄを選別する
	COLLISION_VEC list;
	auto it = m_aIntMap.begin();

	for (; it != m_aIntMap.end(); )
	{
		// null確認
		if (!(*it).second.lock())
		{
			it = m_aIntMap.erase((it));
			continue;
		}

		// 状態確認
		if (!(*it).second.lock()->GetOwner()->IsActive())
		{
			++it;
			continue;
		}

		list.push_back((*it).second);	// これ忘れると意味ない(やらかし1)
		++it;
	}

	// 最初以外
	for (int cnt = 0; cnt < static_cast<int>(list.size()); ++cnt)
	{
		for (int otherCnt = cnt + 1; otherCnt < static_cast<int>(list.size()); ++otherCnt)
		{
			if (list[cnt].lock() == list[otherCnt].lock())
				continue;

			// 自身のコリジョンｺﾝﾎﾟｰﾈﾝﾄを引き渡す
			list[cnt].lock()->HitCheckPtr(list[otherCnt].lock().get());
		}
	}

	// 離れたオブジェクトを確認
	// 配列からの除外
	for (it = m_aIntMap.begin(); it != m_aIntMap.end();)
	{			
		// Active確認
		if (!(*it).second.lock()->IsActive())
		{
			it++;
			continue;
		}

		// 離れた
		(*it).second.lock()->ExitTell();
		it++;
	}

}
