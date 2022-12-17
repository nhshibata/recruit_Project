//=========================================================
// [instantResourceManager.cpp]
// 作成:2022/07/31
// シーンのResourceを一時的に管理する
// セーブ・ロードを行う
//=========================================================

//--- インクルード部
#include <GameSystem/Manager/instantResourceManager.h>
#include <GraphicsSystem/Manager/imageResourceManager.h>
#include <GraphicsSystem/Manager/effectManager.h>
#include <GraphicsSystem/Manager/modelManager.h>

using namespace MySpace::System;
using namespace MySpace::Graphics;
using namespace MySpace::Game;

void CInstantResourceManager::Load()
{
	// ﾃｸｽﾁｬ読み込み
	for (auto it = m_aTexList.begin(); it != m_aTexList.end(); ++it)
	{
		CImageResourceManager::Get()->Load(*it);
	}
	
	// モデル読み込み
	for (auto it = m_aModelList.begin(); it != m_aModelList.end(); ++it)
	{
		CModelManager::Get()->Load(*it);
	}

	// エフェクト読み込み
	for (auto it = m_aEffectList.begin(); it != m_aEffectList.end(); ++it)
	{
		CEffekseer::Get()->Load(*it);
	}
}
void CInstantResourceManager::Save()
{
	// 引き渡し用に格納
	m_aTexList = CImageResourceManager::Get()->GetNameList();
	m_aModelList = CModelManager::Get()->GetNameList();
	m_aEffectList = CEffekseer::Get()->GetNameList();
}
void CInstantResourceManager::SceneUnload()
{
	CImageResourceManager::Get()->SceneUnload();
	CModelManager::Get()->SceneUnload();
	//CEffekseer::Get()->GetNameList();
}
