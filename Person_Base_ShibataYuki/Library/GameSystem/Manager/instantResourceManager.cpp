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
	for (auto it = m_TexList.begin(); it != m_TexList.end(); ++it)
	{
		CImageResourceManager::Get().Load(*it);
	}
	
	// モデル読み込み
	for (auto it = m_ModelList.begin(); it != m_ModelList.end(); ++it)
	{
		CModelManager::Get().Load(*it);
	}

	// エフェクト読み込み
	for (auto it = m_EffectList.begin(); it != m_EffectList.end(); ++it)
	{
		CEffekseer::Get().Load(*it);
	}
}
void CInstantResourceManager::Save()
{
	// 引き渡し用に格納
	m_TexList = CImageResourceManager::Get().GetNameList();
	m_ModelList = CModelManager::Get().GetNameList();
	m_EffectList = CEffekseer::Get().GetNameList();
}