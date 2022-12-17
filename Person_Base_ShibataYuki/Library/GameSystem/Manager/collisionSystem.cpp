//=====================================================
// [collisionSystem.cpp]
// 作成:2022/11/9
//----------------------------
//
//=====================================================

//--- インクルード部
#include <GameSystem/Manager/collisionSystem.h>
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Component/Collision/collision.h>

using namespace MySpace::Game;

CCollisionSystem::CCollisionSystem()
{
}
CCollisionSystem::~CCollisionSystem()
{
	m_aIntMap.clear();
}
// REVIEW: 改善の余地あり
void CCollisionSystem::CollisionCheck()
{
	if (m_aIntMap.size() == 0)
		return;
	
	//--- 当たり判定を行うｺﾝﾎﾟｰﾈﾝﾄを選別する
	COLLISION_VEC list;
	auto it = m_aIntMap.begin();
	auto first_col = (*it).second;

	// 最初がnullなら探索
	if (!first_col.lock())
	{
		for (it = m_aIntMap.begin(); it != m_aIntMap.end(); ++it)
		{
			if ((*it).second.lock())
				first_col = (*it).second;
		}
	}
	list.push_back(first_col);

	for (auto it = m_aIntMap.begin(); it != m_aIntMap.end(); ++it)
	{
		// 同一のオブジェクトは判定しない
		if (first_col.lock() == (*it).second.lock())
			continue;

		// 破棄されていないか確認
		if (!(*it).second.lock()->GetOwner()->IsActive() || !(*it).second.lock())
		{
			continue;
		}
#ifdef _DEBUG
		auto name = first_col.lock()->GetOwner()->GetName();
		auto flg = first_col.lock()->GetOwner()->GetState();
		auto other_name = (*it).second.lock()->GetOwner()->GetName();
		auto other_flg = (*it).second.lock()->GetOwner()->GetState();
		// 自身のコリジョンｺﾝﾎﾟｰﾈﾝﾄを引き渡す
		bool res = first_col.lock()->HitCheckPtr((*it).second.lock().get());
		list.push_back((*it).second);
#else
		// 自身のコリジョンｺﾝﾎﾟｰﾈﾝﾄを引き渡す
		first_col.lock()->HitCheckPtr((*it).second.lock().get());
		list.push_back((*it).second);	// これ忘れると意味ない(やらかし1)
#endif

	}
	// 最初以外
	for (int cnt = 1; cnt < static_cast<int>(list.size()); ++cnt)
	{
		for (int otherCnt = 0; otherCnt < static_cast<int>(list.size()); ++otherCnt)
		{
			if (list[cnt].lock() == list[otherCnt].lock())
				continue;
#ifdef _DEBUG
			auto name = list[cnt].lock()->GetOwner()->GetName();
			auto flg = list[cnt].lock()->GetOwner()->GetState();
			auto other_name = list[otherCnt].lock()->GetOwner()->GetName();
			auto other_flg = list[otherCnt].lock()->GetOwner()->GetState();
			// 自身のコリジョンｺﾝﾎﾟｰﾈﾝﾄを引き渡す
			bool res = list[cnt].lock()->HitCheckPtr(list[otherCnt].lock().get());
#else
			// 自身のコリジョンｺﾝﾎﾟｰﾈﾝﾄを引き渡す
			list[cnt].lock()->HitCheckPtr(list[otherCnt].lock().get());
#endif // _DEBUG
		}
	}

	// 離れたオブジェクトを確認
	// 配列からの除外
	for (it = m_aIntMap.begin(); it != m_aIntMap.end();)
	{	
		// null確認
		if (!(*it).second.lock())
		{
			it = m_aIntMap.erase((it));
			continue;
		}
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
