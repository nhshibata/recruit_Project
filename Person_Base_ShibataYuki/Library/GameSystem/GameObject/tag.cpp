//==========================================================
// [tag.cpp]
// 作成:2022/06/15 静的メンバ変数を宣言(しないとエラーが起きる)
//==========================================================

//--- インクルード部
#include <GameSystem/GameObject/tag.h>
#include <Application/screen.h>

#include <CoreSystem/Util/define.h>
#include <CoreSystem/File/cerealize.h>

#include <DebugSystem/imGuiPackage.h>

using namespace MySpace::Game;
using namespace MySpace::System;


//==========================================================
// コンストラクタ
//==========================================================
CTag::CTag()
	:m_nTagID(0)
{
	CreateTag(CTagDefault::DEFAULT);
	SetTag(CTagDefault::DEFAULT);
}

//==========================================================
// コンストラクタ
//==========================================================
CTag::CTag(std::string name)
{
	CreateTag(name);
	m_nTagID = FindIdx(name);
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

//==========================================================
// 表示
//==========================================================
void CTag::ImGuiTag(bool& disp)
{
	static std::string newTagName;

	if (!disp)
		return;
	
	ImGui::SetNextWindowPos(ImGui::GetMousePos(), ImGuiCond_::ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(CScreen::GetWidth() / 8, CScreen::GetHeight() / 6), ImGuiCond_::ImGuiCond_Once);
	if (ImGui::Begin("Create Tag", &disp))
	{
		char input[52];
		strcpy_s(input, newTagName.c_str());
		Debug::SetTextAndAligned("New Tag >>");
		if (ImGui::InputText("input", input, 52))
		{
			newTagName = input;
		}

		if (ImGui::Button("Create"))
		{
			CTag::CreateTag(newTagName);
			newTagName.clear();
			disp = false;
		}
		ImGui::End();
	}

}

