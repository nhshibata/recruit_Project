//=========================================================
// [imageResourceManager.cpp] 
// 作成:2022/06/19
//---------------------------------------------------------
//=========================================================

//--- インクルード部
#include <GraphicsSystem/Manager/imageResourceManager.h>

using namespace MySpace::System;
using namespace MySpace::Graphics;

//--- 定数定義


CImageResourceManager::CImageResourceManager()
{
}
void CImageResourceManager::Uninit()
{
	UnloadAll();
}
bool CImageResourceManager::Load(std::string name)
{
	// 読み込み済みか確認
	auto it = m_aResourceMap.find(name);
	if (it != m_aResourceMap.end())
		return true;
	// 作成
	std::shared_ptr<CImageResource> tex = std::make_shared<CImageResource>();

	// 読み込み
	if (!tex->Load(name)) 
	{
		return false;
	}
	// 格納
	m_aResourceMap.insert(IMAGE_PAIR(name, tex));
	return true;
}
bool CImageResourceManager::Unload(std::string name)
{
	auto it = m_aResourceMap.find(name);
	if (it != m_aResourceMap.end())
	{
		m_aResourceMap[name].reset();	// 所有権の破棄
		m_aResourceMap.erase(name);		// リストからの除外
		return true;
	}
	return false;
}
void CImageResourceManager::UnloadAll()
{
	for (auto it = m_aResourceMap.begin(); it != m_aResourceMap.end(); ++it)
	{
		it->second.reset();
	}
	m_aResourceMap.clear();
}
// 画像を渡す
ImageSharedPtr CImageResourceManager::GetResource(std::string name)
{
	auto it = m_aResourceMap.find(name);
	if (it == m_aResourceMap.end())
	{
		if (!Load(name))
		{
			// 読み込み失敗
			return std::shared_ptr<CImageResource>();
		}
	}

	return m_aResourceMap[name];
}
int CImageResourceManager::SceneUnload()
{
#if _DEBUG	// 確認
	int cnt = static_cast<int>(m_aResourceMap.size());
	for (auto & image : m_aResourceMap)
	{
		// 所持権が自身しか持っていなければ解放
		if (image.second.use_count() == 1)
		{
			image.second->Unload();
			image.second.reset();
			--cnt;
		}
	}
	return cnt;
#else
	for (auto & image : m_ResourceMap)
	{
		// 所持権が自身しか持っていなければ解放
		if (image.second.use_count() == 1)
		{
			image.second->Unload();
			image.second.reset();
		}
	}
#endif // _DEBUG

	return 0;
}