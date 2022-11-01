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
	for (auto it = m_TexList.begin(); it != m_TexList.end(); ++it)
	{
		CImageResourceManager::Get().Load(*it);
	}
	
	// ���f���ǂݍ���
	for (auto it = m_ModelList.begin(); it != m_ModelList.end(); ++it)
	{
		CModelManager::Get().Load(*it);
	}

	// �G�t�F�N�g�ǂݍ���
	for (auto it = m_EffectList.begin(); it != m_EffectList.end(); ++it)
	{
		CEffekseer::Get().Load(*it);
	}
}
void CInstantResourceManager::Save()
{
	// �����n���p�Ɋi�[
	m_TexList = CImageResourceManager::Get().GetNameList();
	m_ModelList = CModelManager::Get().GetNameList();
	m_EffectList = CEffekseer::Get().GetNameList();
}