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
	auto it = m_ResourceMap.find(name);
	if (it != m_ResourceMap.end())
		return true;
	// 作成
	std::shared_ptr<CImageResource> tex = std::make_shared<CImageResource>();

	// 読み込み
	if (!tex->Load(name)) 
	{
		return false;
	}
	// 格納
	m_ResourceMap.insert(IMAGE_PAIR(name, tex));
	return true;
}
bool CImageResourceManager::Unload(std::string name)
{
	auto it = m_ResourceMap.find(name);
	if (it != m_ResourceMap.end())
	{
		m_ResourceMap[name].reset();	// 所有権の破棄
		m_ResourceMap.erase(name);		// リストからの除外
		return true;
	}
	return false;
}
void CImageResourceManager::UnloadAll()
{
	for (auto it = m_ResourceMap.begin(); it != m_ResourceMap.end(); ++it)
	{
		it->second.reset();
	}
	m_ResourceMap.clear();
}
// 画像を渡す
ImageSharedPtr CImageResourceManager::GetResource(std::string name)
{
	auto it = m_ResourceMap.find(name);
	if (it == m_ResourceMap.end())
	{
		if (!Load(name))
		{
			// 読み込み失敗
			return std::shared_ptr<CImageResource>();
		}
	}

	return m_ResourceMap[name];
}
