//=========================================================
// [imageResourceManager.cpp] 
// 作成:2022/06/19
//---------------------------------------------------------
//=========================================================

//--- インクルード部
#include <Application/Application.h>
#include <GraphicsSystem/Manager/modelManager.h>
#include <CoreSystem/Util/define.h>

using namespace MySpace::System;
using namespace MySpace::Graphics;

//==========================================================
// コンストラクタ
//==========================================================
CModelManager::CModelManager()
{
}

//==========================================================
// デストラクタ
//==========================================================
CModelManager::~CModelManager()
{
	UnloadAll();
}

//==========================================================
// 読み込み
//==========================================================
bool CModelManager::Load(std::string name)
{
	// メモリ確保
	ModelSharedPtr addModel = std::make_shared<CAssimpModel>();

	// モデル読み込み
	if (!addModel->Load(Application::Get()->GetDevice(), Application::Get()->GetDeviceContext(),
		name))
	{
		return false;
	}
	m_aResourceMap.insert(CModelManager::Pair(name, addModel));

	return true;
}

//==========================================================
// ﾃﾞｰﾀ破棄
//==========================================================
bool CModelManager::Unload(std::string name)
{
	if (!m_aResourceMap.count(name))
		return false;

	m_aResourceMap[name]->Release();
	m_aResourceMap[name].reset();
	
	//--- リストから破棄
	auto it = m_aResourceMap.find(name);
	m_aResourceMap.erase(it);
	return true;
}

//==========================================================
// 全開放
//==========================================================
void CModelManager::UnloadAll()
{
	for (auto it = m_aResourceMap.begin(); it != m_aResourceMap.end(); ++it)
	{
		(*it).second->Release();
		(*it).second.reset();
	}
	m_aResourceMap.clear();
}

//==========================================================
// シーン破棄時確認
//==========================================================
int CModelManager::SceneUnload()
{
#if _DEBUG	// 確認
	int cnt = static_cast<int>(m_aResourceMap.size());
	for (auto it = m_aResourceMap.begin(); it != m_aResourceMap.end();)
	{
		// 所持権が自身だけなら解放
		if ((*it).second.use_count() == 1)
		{
			(*it).second->Release();
			(*it).second.reset();
			it = m_aResourceMap.erase(it);
			--cnt;
			continue;
		}
		++it;
	}
	return cnt;

#else
	for (auto it = m_aResourceMap.begin(); it != m_aResourceMap.end();)
	{
		// 所持権が自身だけなら解放
		if ((*it).second.use_count() == 1)
		{
			(*it).second->Release();
			(*it).second.reset();
			it = m_aResourceMap.erase(it);
			continue;
		}
		++it;
	}
	
#endif // _DEBUG

	return 0;
}

//==========================================================
// モデル取得
// なければ読み込み
//==========================================================
ModelSharedPtr CModelManager::GetModel(std::string name)
{
	// 存在するか確認
	if (auto it = m_aResourceMap.find(name); it == m_aResourceMap.end())
	{
		if (!Load(name))
		{
			return ModelSharedPtr();
		}
	}
	return m_aResourceMap[name];
};

//==========================================================
// モデル使用数取得
//==========================================================
int CModelManager::GetModelCnt(std::string name)
{
	// 読みこんでいるか
	if (m_aResourceMap.count(name))
	{
		return m_aResourceMap[name].use_count();
	}
	return 0;
}

//==========================================================
// 使用終了後呼び出し
//==========================================================
void CModelManager::FinishUse(std::string name)
{
	// そもそも読みこんでいるか
	if (m_aResourceMap.count(name))
	{
		// 管理ｸﾗｽしか所持者が居ないか
		if (m_aResourceMap[name].use_count() == 1)
		{
			auto it = m_aResourceMap.find(name);
			(*it).second->Release();
			(*it).second.reset();
			it = m_aResourceMap.erase(it);
		}
	}
}