//==========================================================
// [tag.cpp]
// 作成:2022/06/15 静的メンバ変数を宣言(しないとエラーが起きる)
//==========================================================

//--- インクルード部
#include <GameSystem/GameObject/tag.h>

#include <CoreSystem/Util/define.h>
#include <CoreSystem/File/cerealize.h>

using namespace MySpace::Game;
using namespace MySpace::System;

//--- 定数定義
#define TAG_PATH		FORDER_DIR(data/SystemData/tag.json)

//--- 静的メンバ変数

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
// コンストラクタ
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


