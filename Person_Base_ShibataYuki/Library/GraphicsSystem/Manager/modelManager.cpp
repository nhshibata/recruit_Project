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

//--- 定数定義

//--- 静的メンバ変数
namespace {
	/*static const char* g_pszModelPath[(int)EModelType::MAX] =
	{
		FORDER_DIR(Data/model/hover.fbx),
		FORDER_DIR(Data/model/SkyDome/sky.fbx),
		FORDER_DIR(Data/model/land.fbx),
	};*/
}

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