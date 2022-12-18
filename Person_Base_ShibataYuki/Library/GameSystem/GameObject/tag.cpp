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

// コンストラクタ
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


