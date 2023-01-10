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

//==========================================================
// コンストラクタ
//==========================================================
CTag::CTag()
	:m_nTagID(0)
{
	CreateTag(CDefaultTagChar::DEFAULT);
	SetTag(CDefaultTagChar::DEFAULT);
}

//==========================================================
// コンストラクタ
//==========================================================
CTag::CTag(std::string name)
{
	CreateTag(name);
	m_nTagID = GetID(name);
}

//==========================================================
// デストラクタ
//==========================================================
CTag::~CTag()
{

}

//==========================================================
// ファイル出力
//==========================================================
void CTag::SaveSystem()
{
	CCerealize<std::vector<std::string>> sirial;
	sirial.OutputFile("tagList", TAG_PATH, m_aTagName);
}

//==========================================================
// ファイル読み込み
//==========================================================
void CTag::LoadSystem()
{
	CCerealize<std::vector<std::string>> sirial;
	m_aTagName = sirial.InputFile(TAG_PATH);
}

