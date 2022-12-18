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

// �R���X�g���N�^
CTag::CTag()
	:m_nTagID(0)
{
	CreateTag(CDefaultTagChar::DEFAULT);
	SetTag(CDefaultTagChar::DEFAULT);
}

CTag::CTag(std::string name)
{
	CreateTag(name);
	m_nTagID = GetID(name);
}

CTag::~CTag()
{

}


