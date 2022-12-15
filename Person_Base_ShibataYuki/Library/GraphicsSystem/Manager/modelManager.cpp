//=========================================================
// [imageResourceManager.cpp] 
// 作成:2022/06/19
//---------------------------------------------------------
//=========================================================

//--- インクルード部
#include <GraphicsSystem/DirectX/DXDevice.h>
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

CModelManager::CModelManager()
{

}
HRESULT CModelManager::Init()
{
	if (!CAssimpModel::InitShader(CDXDevice::Get().GetDevice()))
	{
		return E_FAIL;
	}
	return S_OK;
}
void CModelManager::Uninit()
{
	UnloadAll();

	// Assimp用シェーダ終了処理
	CAssimpModel::UninitShader();
}
bool CModelManager::Load(std::string name)
{
	ModelSharedPtr addModel = std::make_shared<CAssimpModel>();
	// モデル読み込み
	if (!addModel->Load(CDXDevice::Get().GetDevice(), CDXDevice::Get().GetDeviceContext(),
		name))
	{
		return false;
	}
	m_aResourceMap.insert(CModelManager::Pair(name, addModel));

	return true;
}
bool CModelManager::Unload(std::string name)
{
	m_aResourceMap[name]->Release();
	m_aResourceMap[name].reset();
	return true;
}
void CModelManager::UnloadAll()
{
	for (auto it = m_aResourceMap.begin(); it != m_aResourceMap.end(); ++it)
	{
		(*it).second->Release();
		(*it).second.reset();
	}
	m_aResourceMap.clear();
}

int CModelManager::SceneUnload()
{
#if _DEBUG	// 確認
	int cnt = static_cast<int>(m_aResourceMap.size());
	for (auto & image : m_aResourceMap)
	{
		// 所持権が自身しか持っていなければ解放
		if (image.second.use_count() == 1)
		{
			image.second->Release();
			image.second.reset();
			--cnt;
		}
	}
	return cnt;
#else
	for (auto & image : m_aResourceMap)
	{
		// 所持権が自身しか持っていなければ解放
		if (image.second.use_count() == 1)
		{
			image.second->Release();
			image.second.reset();
		}
	}
#endif // _DEBUG

	return 0;
}