//=========================================================
// [instantResourceManager.cpp]
// 作成:2022/07/31
// シーンのResourceを一時的に管理する
// セーブ・ロードを行う
//=========================================================

//--- インクルード部
#include <Application/Application.h>
#include <GraphicsSystem/Manager/assetsManager.h>
#include <GraphicsSystem/Manager/imageResourceManager.h>
#include <GraphicsSystem/Manager/effectManager.h>
#include <GraphicsSystem/Manager/modelManager.h>
#include <GameSystem/Manager/instantResourceManager.h>

using namespace MySpace::System;
using namespace MySpace::Graphics;
using namespace MySpace::Game;

//========================================================
// 読み込み
//========================================================
void CInstantResourceManager::Load()
{
	auto pAssets = Application::Get()->GetSystem<CAssetsManager>();
	
	// ﾃｸｽﾁｬ読み込み
	for (auto it = m_aTexList.begin(); it != m_aTexList.end(); ++it)
	{
		pAssets->GetImageManager()->Load(*it);
	}
	
	// モデル読み込み
	for (auto it = m_aModelList.begin(); it != m_aModelList.end(); ++it)
	{
		pAssets->GetModelManager()->Load(*it);
	}

	// エフェクト読み込み
	for (auto it = m_aEffectList.begin(); it != m_aEffectList.end(); ++it)
	{
		pAssets->GetEffekseer()->Load(*it);
	}
}

//========================================================
// 保存用
//========================================================
void CInstantResourceManager::Save()
{
	auto pAssets = Application::Get()->GetSystem<CAssetsManager>();

	// 引き渡し用に格納
	m_aTexList = pAssets->GetImageManager()->GetNameList();
	m_aModelList = pAssets->GetModelManager()->GetNameList();
	m_aEffectList = pAssets->GetEffekseer()->GetNameList();
}

//========================================================
// Scene解放時
//========================================================
void CInstantResourceManager::SceneUnload()
{
	auto pAssets = Application::Get()->GetSystem<CAssetsManager>();
	if (!pAssets)
		return;

	pAssets->GetImageManager()->SceneUnload();
	pAssets->GetModelManager()->SceneUnload();
	//CEffekseer::Get()->GetNameList();
}
