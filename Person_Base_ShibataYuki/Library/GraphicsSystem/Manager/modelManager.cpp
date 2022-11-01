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
	m_ResourceMap.insert(CModelManager::Pair(name, addModel));

	return true;
}
bool CModelManager::Unload(std::string name)
{
	m_ResourceMap[name]->Release();
	m_ResourceMap[name].reset();
	return true;
}
void CModelManager::UnloadAll()
{
	for (auto it = m_ResourceMap.begin(); it != m_ResourceMap.end(); ++it)
	{
		(*it).second->Release();
		(*it).second.reset();
	}
}