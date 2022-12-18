//=========================================================
// [tagManager.cpp]
//--------------------
// �쐬:2022/12/18 tag.h���番��
//--------------------
//=========================================================

//--- �C���N���[�h��
#include <GameSystem/Manager/tagManager.h>

#include <CoreSystem/Util/define.h>
#include <CoreSystem/File/cerealize.h>
#include <CoreSystem/Util/cerealCommon.h>

using namespace MySpace::Game;
using namespace MySpace::System;

//--- �萔��`
#define TAG_PATH		FORDER_DIR(data/SystemData/tag.json)

//--- �ÓI�����o�ϐ�

CTagManager::CTagManager()
{
	m_aTagName.clear();
	//SetLoad();
	//SetSave();
}

CTagManager::~CTagManager()
{
	SaveSystem();
}
void CTagManager::SaveSystem()
{
	CCerealize<std::vector<std::string>> sirial;
	sirial.OutputFile("tagList", TAG_PATH, m_aTagName);
}
void CTagManager::LoadSystem()
{
	CCerealize<std::vector<std::string>> sirial;
	m_aTagName = sirial.InputFile(TAG_PATH); 
}

