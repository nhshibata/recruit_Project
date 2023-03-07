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