//==========================================================
// [volumeManager.cpp]
//
//---------------------------------------------------------
//==========================================================

//--- インクルード部
#include <GameSystem/Manager/volumeManager.h>
#include <algorithm>

using namespace MySpace::Game;

//==========================================================
// コンストラクタ
//==========================================================
CVolumeManager::CVolumeManager()
{

}

//==========================================================
// デストラクタ
//==========================================================
CVolumeManager::~CVolumeManager()
{

}

//==========================================================
// レイヤーと一致するvolumeを格納し、返す
//==========================================================
std::vector<CVolume*> CVolumeManager::GetVolume(const int layer)const
{
	std::vector<CVolume*> ret;
	
	//--- 一致するものを格納
	for (auto & vol : m_aIntMap)
	{
		if (!vol.second->GetEffect())
			continue;

		if (vol.second->IsLayer(layer))
		{
			ret.push_back(vol.second);
		}
	}

	//--- 優先度で昇順に並び替え
	std::sort(ret.begin(), ret.end(), [](auto const& a, auto const& b)->bool {
		return a->GetPriority() < b->GetPriority();
	});

	return ret;
}

//=========================================================
// 存在するLayer
//=========================================================
const int CVolumeManager::GetBit(const int layer)
{
	int retBit = 0;

	//--- 一致するものを格納
	for (auto & vol : m_aIntMap)
	{
		// エフェクトが設定されていない
		if (!vol.second->GetEffect())
			continue;

		// レイヤーが一致しない
		if (!vol.second->IsLayer(layer))
			continue;

		//--- 条件達成
		// GameObjectのLayer番号を取得して、Bit変換
		// Bitを立てる
		retBit |= CLayer::NumberToBit(vol.second->GetLayer());
	}

	return retBit;
}

//==========================================================
// レイヤーと一致したvolumeへIDを追加する
//==========================================================
void CVolumeManager::AddRendererCache(const int nLayer, const int nID)
{
	//--- 一致するものを格納
	for (auto & vol : m_aIntMap)
	{
		// レイヤーが一致しない
		if (!vol.second->IsLayer(nLayer))
			continue;

		// IDを追加
		vol.second->AddRendererID(nID);
	}

}

//==========================================================
// 所持しているVolumeコンポーネントのキャッシュをクリア
//==========================================================
void CVolumeManager::ResetRendererCache()
{
	for (auto & vol : m_aIntMap)
	{
		vol.second->ResetRenderCache();
	}
}