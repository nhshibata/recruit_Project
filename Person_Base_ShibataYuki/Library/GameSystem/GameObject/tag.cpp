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

//==========================================================
// �R���X�g���N�^
//==========================================================
CTag::CTag()
	:m_nTagID(0)
{
	CreateTag(CDefaultTagChar::DEFAULT);
	SetTag(CDefaultTagChar::DEFAULT);
}

//==========================================================
// �R���X�g���N�^
//==========================================================
CTag::CTag(std::string name)
{
	CreateTag(name);
	m_nTagID = GetID(name);
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CTag::~CTag()
{

}

//==========================================================
// �t�@�C���o��
//==========================================================
void CTag::SaveSystem()
{
	CCerealize<std::vector<std::string>> sirial;
	sirial.OutputFile("tagList", TAG_PATH, m_aTagName);
}

//==========================================================
// �t�@�C���ǂݍ���
//==========================================================
void CTag::LoadSystem()
{
	CCerealize<std::vector<std::string>> sirial;
	m_aTagName = sirial.InputFile(TAG_PATH);
}

