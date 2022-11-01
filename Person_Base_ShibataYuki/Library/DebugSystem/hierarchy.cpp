//=========================================================
// [hirarchy.cpp] 
// �쐬:2022/07/10
//---------------------------------------------------------
//=========================================================


//--- �C���N���[�h��
#include <DebugSystem/hierarchy.h>
#if BUILD_MODE

#include <DebugSystem/imguiManager.h>
#include <DebugSystem/inspector.h>
#include <DebugSystem/imGuiPackage.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_win32.h>
#include <ImGui/imgui_impl_dx11.h>

#include <GameSystem/Scene/scene.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/Manager/sceneManager.h>
#include <GameSystem/Manager/gameObjectManager.h>
#include <GameSystem/Component/Camera/debugCamera.h>
#include <CoreSystem/File/filePath.h>

using namespace MySpace::System;
using namespace MySpace::Debug;
using namespace MySpace::SceneManager;

CHierachy::CHierachy()
	:m_bLoadSaveWindow(false)
{
	m_loadPath = FORDER_DIR(Data/scene);
	LoadScenePathList();
}
CHierachy::~CHierachy()
{
}
void CHierachy::Init()
{
	LoadScenePathList();
}
void CHierachy::Uninit()
{
}
void CHierachy::Update()
{
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(200, 400), ImGuiCond_Once);
	bool flg = true;
	ImGui::Begin("Hierarchy", &flg, ImGuiWindowFlags_MenuBar);

	if (!CSceneManager::Get().GetActiveScene())
		return;

	auto objList = CSceneManager::Get().GetActiveScene()->GetObjManager()->GetList();

	if (ImGui::BeginMenuBar())
	{
		// �t�@�C��
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Scene"))
			{
				CSceneManager::Get().CreateNewScene<CScene>();
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Load/Save"))
			{
				m_bLoadSaveWindow = !m_bLoadSaveWindow;
			}
			ImGui::EndMenu();
		}
		// �I�u�W�F�N�g����
		if (ImGui::BeginMenu("GameObject"))
		{
			if (ImGui::MenuItem("Empty"))
			{
				auto obj = CSceneManager::Get().GetActiveScene()->GetObjManager()->CreateGameObject();
				ImGuiManager::Get().GetInspector()->SetGameObject(obj);
			}
			/*if (ImGui::MenuItem("Plane"))
			{
				auto obj = CGameObject::CreateObject();
				ImGuiManager::Get().GetInspector()->SetGameObject(obj);
			}*/
			if (ImGui::MenuItem("DebugCamera"))
			{
				auto obj = CGameObject::CreateObject();
				obj.lock()->AddComponent<CDebugCamera>();
				// ���߰��Ă̒ǉ���
				ImGuiManager::Get().GetInspector()->SetGameObject(obj);
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	// �����X�V
	UpdateSearch();

	bool select = false;
	// �Q�[���I�u�W�F�N�g���̕\���Ǝq�v�f�̕\��
	for (const auto & object : objList)
	{
		// ������ON�ɂȂ��Ă����
		if (m_Search.bSearchCriteria)
		{	// ���������ƈ�v���邩����
			if (!DispCheck(object.get()))
				continue;
		}
#if 1
		// �e�v�f�̊m�F
		if (object->GetTransform()->GetParent().lock())
			continue;
#endif // 0
		// �E�B���h�E�\���{�^��
		if (ImGui::Button(object->GetName().c_str()) )
		{
			ImGuiManager::Get().GetInspector()->SetGameObject(object);
			break;
		}
		ImGui::SameLine();

		if (ImGui::TreeNode(std::string("child-" + object->GetName()).c_str() ))
		{
			if (object->GetTransform()->GetChild(0).expired())
			{
				ImGui::Text(u8"�Ȃ�");
			}
			for (auto cnt = 0; cnt < object->GetTransform()->GetChildCount(); ++cnt)
			{
				// nullptr�m�F
				if (auto child = object->GetTransform()->GetChild(cnt).lock() ; child)
				{
					std::string ownname = child->GetName();
					const char* childName = ownname.c_str();

					ImGui::BeginChild(childName, ImVec2(0, 30), false);
					ImGui::SameLine();
					if (select = ImGui::Button(childName); select)
					{
						ImGuiManager::Get().GetInspector()->SetGameObject(child);
						
					}
					ImGui::EndChild();
					ImGui::Separator();
					
					if (select)break;
				}
			}
			ImGui::TreePop();
		}
	}
	ImGui::End();

	// �Z�[�u���[�h
	if (m_bLoadSaveWindow)
	{
		DispSaveLoadMenu();
	}

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}
void CHierachy::LoadScenePathList()
{
	CFilePath path;
	m_scenePathList = path.GetFileName(m_loadPath);
}
void CHierachy::DispSaveLoadMenu()
{
	bool flg = true;
	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(200, 300), ImGuiCond_Once);

	ImGui::Begin("Load", &flg, ImGuiWindowFlags_MenuBar);

	//
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("sceneList"))
		{
			for (auto it = m_scenePathList.begin(); it < m_scenePathList.end(); ++it)
			{
				auto name = (*it).c_str();
				if (ImGui::MenuItem(name)) {
					m_loadPath = name;
				}
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	if (ImGui::Button("pathReload")) {
		LoadScenePathList();
	}

	//ImGui::InputText("reloadFile", m_loadPath.data(), 256);
	InputString(m_loadPath, "loadFile");

	if (ImGui::Button("Load"))
	{
		CSceneManager::Get().LoadScene(m_loadPath);
	}
	ImGui::Separator();

	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(200, 300), ImGuiCond_Once);

	//ImGui::Begin("Save", &batsu, ImGuiWindowFlags_MenuBar);
	InputString(m_savePath, "saveFile");
	if (ImGui::Button("Save"))
	{
		CSceneManager::Get().SaveScene(m_savePath);
	}
	ImGui::End();
}
#pragma region SEARCH
void CHierachy::UpdateSearch()
{
	// for���ϐ�
	const static int nSearch[static_cast<int>(ESearchTerms::MAX)] =
	{
		static_cast<int>(ESearchTerms::OBJ_NAME),
		static_cast<int>(ESearchTerms::TAG),
		static_cast<int>(ESearchTerms::STATE_ACTIVE),
		static_cast<int>(ESearchTerms::STATE_STOP),
		static_cast<int>(ESearchTerms::STATE_DESTROY),
	};
	static const char* szDisp[static_cast<int>(ESearchTerms::MAX)] =
	{
		"objName","objTag","stateActiv","stateStop","stateDestroy"
	};

	ImGui::Text((m_Search .bSearchCriteria? "����ON": "����OFF"));
	ImGui::SameLine();
	if (ImGui::Button("����"))
	{
		m_Search.bSearchCriteria ^= true;
	}
	ImGui::SameLine();
	InputString(m_Search.inputName, "��������");

	for (int cnt = 0; cnt < static_cast<int>(ESearchTerms::MAX); ++cnt)
	{
		ImGui::RadioButton(szDisp[cnt], (int*)&m_Search.eTerms, nSearch[cnt]);
		ImGui::SameLine();
	}
}
// �\������Ώۂ��m�F
bool CHierachy::DispCheck(CGameObject* obj)
{
	switch (m_Search.eTerms)
	{
	case MySpace::Debug::CHierachy::ESearchTerms::OBJ_NAME:
		// �ꕔ��v
		if (obj->GetName().find(m_Search.inputName) != std::string::npos)
		{
			return true;
		}
		break;
	case MySpace::Debug::CHierachy::ESearchTerms::TAG:
		// tag��r
		return obj->GetTagPtr()->Compare(m_Search.inputName);
	case MySpace::Debug::CHierachy::ESearchTerms::STATE_ACTIVE:
		return obj->GetState() == CGameObject::E_ObjectState::ACTIVE;
	case MySpace::Debug::CHierachy::ESearchTerms::STATE_STOP:
		return obj->GetState() == CGameObject::E_ObjectState::STOP;
	case MySpace::Debug::CHierachy::ESearchTerms::STATE_DESTROY:
		return obj->GetState() == CGameObject::E_ObjectState::DESTROY;
	default:
		break;
	}
	return false;
}
#pragma endregion

#endif // !BUILD_MODE
