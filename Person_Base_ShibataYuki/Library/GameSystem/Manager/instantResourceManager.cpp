//=========================================================
// [instantResourceManager.cpp]
// �쐬:2022/07/31
// �V�[����Resource���ꎞ�I�ɊǗ�����
// �Z�[�u�E���[�h���s��
//=========================================================

//--- �C���N���[�h��
#include <GameSystem/Manager/instantResourceManager.h>
#include <GraphicsSystem/Manager/imageResourceManager.h>
#include <GraphicsSystem/Manager/effectManager.h>
#include <GraphicsSystem/Manager/modelManager.h>

using namespace MySpace::System;
using namespace MySpace::Graphics;
using namespace MySpace::Game;

void CInstantResourceManager::Load()
{
	// ø����ǂݍ���
	for (auto it = m_aTexList.begin(); it != m_aTexList.end(); ++it)
	{
		CImageResourceManager::Get()->Load(*it);
	}
	
	// ���f���ǂݍ���
	for (auto it = m_aModelList.begin(); it != m_aModelList.end(); ++it)
	{
		CModelManager::Get()->Load(*it);
	}

	// �G�t�F�N�g�ǂݍ���
	for (auto it = m_aEffectList.begin(); it != m_aEffectList.end(); ++it)
	{
		CEffekseer::Get()->Load(*it);
	}
}
void CInstantResourceManager::Save()
{
	// �����n���p�Ɋi�[
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
