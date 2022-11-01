//==========================================================
// [tag.cpp]
// �쐬:2022/06/15 �ÓI�����o�ϐ���錾(���Ȃ��ƃG���[���N����)
//==========================================================

//--- �C���N���[�h��
#include <GameSystem/GameObject/tag.h>

#include <CoreSystem/Util/define.h>
#include <CoreSystem/File/cerealize.h>

using namespace MySpace::Game;
using namespace MySpace::System;

//--- �萔��`
#define TAG_PATH		FORDER_DIR(data/SystemData/tag.json)

//--- �ÓI�����o�ϐ�

CTagName::CTagName()
{
	//SetLoad();
	//SetSave();
}

void CTagName::SaveSystem()
{
	CCerealize<std::vector<std::string>> sirial;
	sirial.OutputFile("tagList", TAG_PATH, m_tagVec);
}

void CTagName::LoadSystem()
{
	CCerealize<std::vector<std::string>> sirial;
	m_tagVec = sirial.InputFile(TAG_PATH); 
}
//=====================================================
// �R���X�g���N�^
CTag::CTag()
{
	CreateTag("Default");
	SetTag("Default");
	//m_nTagID = GetID("Default");
}

CTag::CTag(std::string name)
{
	CreateTag(name);
	m_nTagID = GetID(name);
}

CTag::~CTag()
{

}


