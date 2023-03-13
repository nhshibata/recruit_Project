//==========================================================
// [tag.cpp]
// �쐬:2022/06/15 �ÓI�����o�ϐ���錾(���Ȃ��ƃG���[���N����)
//==========================================================

//--- �C���N���[�h��
#include <GameSystem/GameObject/tag.h>
#include <Application/screen.h>

#include <CoreSystem/Util/define.h>
#include <CoreSystem/File/cerealize.h>

#include <DebugSystem/imGuiPackage.h>

using namespace MySpace::Game;
using namespace MySpace::System;


//==========================================================
// �R���X�g���N�^
//==========================================================
CTag::CTag()
	:m_nTagID(0)
{
	CreateTag(CTagDefault::DEFAULT);
	SetTag(CTagDefault::DEFAULT);
}

//==========================================================
// �R���X�g���N�^
//==========================================================
CTag::CTag(std::string name)
{
	CreateTag(name);
	m_nTagID = FindIdx(name);
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

//==========================================================
// �\��
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

