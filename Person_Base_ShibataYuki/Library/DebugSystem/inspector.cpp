//=========================================================
// [inspector.cpp] 
// �쐬:2022/07/10
// �X�V:2022/09/11 ��ݽ̫�т̒l���L�[���͂ŕύX����
//---------------------------------------------------------
//=========================================================

#define _CRT_SECURE_NO_WARNINGS

//--- �C���N���[�h��
#include <DebugSystem/inspector.h>

#if BUILD_MODE
#include <DebugSystem/imguiManager.h>
#include <DebugSystem/imGuiPackage.h>
#include <DebugSystem/imGuiContextMenu.h>

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_win32.h>
#include <ImGui/imgui_impl_dx11.h>

#include <GameSystem/Scene/scene.h>
#include <GameSystem/Manager/sceneManager.h>
#include <GameSystem/Manager/gameObjectManager.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/Component/Transform/rectTransform.h>
#include <GameSystem/Factory/componentFactory.h>

#include <CoreSystem/Input/input.h>
#include <CoreSystem/File/cerealize.h>

using namespace MySpace::System;
using namespace MySpace::Game;
using namespace MySpace::SceneManager;
using namespace MySpace::Debug;

//==========================================================
// �R���X�g���N�^
//==========================================================
CInspector::CInspector()
	:m_bIsAddComponent(false), m_isDeleted(false), m_bOpen(true)
{
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CInspector::~CInspector()
{

}

//==========================================================
// �X�V
//==========================================================
void CInspector::Update(ImGuiManager* manager)
{
	if (!m_bOpen && !m_spViewObj.lock())
		return;

	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
	auto screenSize = CScreen::GetSize();
	screenSize.x *= 0.3f;
	ImGui::SetNextWindowPos(ImVec2(CScreen::GetWidth() - screenSize.x, 0), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(screenSize.x, screenSize.y * 0.75f), ImGuiCond_Once);

	m_bOpen = m_spViewObj.lock() ? true : false;
	ImGui::Begin("Inspector", &m_bOpen, ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_::ImGuiWindowFlags_HorizontalScrollbar);

	if (!m_bOpen)
		m_spViewObj.reset();

	if (!m_spViewObj.lock())
	{
		ImGui::End();

		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		return;
	}

	//--- ���j���[�o�[
	DispOptionBar();

	// �I��obj�f�o�b�O�\��
	DispDebugSelectObject();

	//--- ���j���[�\��
	//DispPopUpMenuObject();
	
	manager->HoverStateSet();

	//--- �R���|�[�l���g�ǉ�
	AddComponentWindow();

	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

//==========================================================
// delete
//==========================================================
void CInspector::DeleteObject()
{
	CGameObject::Destroy(m_spViewObj.lock());
	m_spViewObj.reset();
}

//==========================================================
// �I���I�u�W�F�N�g�ݒ�
//==========================================================
void CInspector::SetSelectGameObject(std::weak_ptr<CGameObject> obj)
{
	m_spViewObj = obj;
	
	//m_TransformController.spRect = m_spViewObj.lock()->GetComponent<CRectTransform>();
	m_isDrawInfo.clear();
	int n = static_cast<int>(obj.lock()->GetComponentList().size()) + 1;
	for (int cnt = 0; cnt < n; ++cnt)
	{
		m_isDrawInfo.push_back(false);
	}
}

//==========================================================
// �I�u�W�F�N�gcopy
// TODO:������
//==========================================================
void CInspector::CopyGameObject()
{
	if (!m_spViewObj.lock())
		return;

	//m_spViewObj = CGameObject::CreateObject(m_spViewObj.lock().get());
	// �����Ŏ������n��
	/*auto comList = m_spViewObj.lock()->GetComponentList();
	for (auto & com : comList)
	{
		com->SetOwner(m_spViewObj.lock());
		com->Init();
	}*/

	// �V���A���C�Y�����Ė��O,�߲���Ȃǂ����㏑������΂ł���?
	// �ł���
	// �V���A���C�Y�N���X�쐬
	CCerealize<std::shared_ptr<CGameObject>> sirial;
	{
		// �ް����O���ۑ�
		auto obj = m_spViewObj.lock();
		sirial.OutputFile(m_spViewObj.lock()->GetName(), COPY_DATA_GAME_OBJECT_PATH, obj);
	}

	//// �ꎞ�I�ȃI�u�W�F�N�g����
	//if (auto work = std::make_shared<CGameObject>(); work)
	//{
	//	// �ް��ǂݍ���
	//	work = sirial.InputFile(COPY_DATA_GAME_OBJECT_PATH);

	//	// �V�����I�u�W�F�N�g����
	//	m_spViewObj = CGameObject::CreateObject();
	//	// �ǂ݂��܂ꂽ�R���|�[�l���g�̎󂯓n��
	//	auto comList = work->GetComponentList();
	//	for (auto & com : comList)
	//	{
	//		m_spViewObj.lock()->SetComponent(com);
	//		//--- �`��Ɠ����蔻��N���X�͗v������K�v�����邽�߁AInit���Ăяo��
	//		// NOTE: ����I�Ȃ��̂Ŕėp���Ɍ�����B�����ǂ��Ȃ̂�
	//		if (com->GetName().find("Renderer") != std::string::npos ||
	//			com->GetName().find("Collision") != std::string::npos)
	//		{
	//			com->Awake();
	//			com->Init();
	//		}
	//	}

	//	// �I�u�W�F�N�g�j��
	//	work.reset();
	//}

	m_spViewObj = Debug::CopyGameObject();
}

//==========================================================
// �I�v�V�����\��
//==========================================================
void CInspector::DispOptionBar()
{
	if (ImGui::BeginMenuBar())
	{
		// �I�u�W�F�N�g����
		if (ImGui::BeginMenu("Option"))
		{
			if (ImGui::MenuItem("AddComponent"))
				m_bIsAddComponent = !m_bIsAddComponent;

			// FIXME:�߲����߰���Ă�̂ݖ�肠��
			if (ImGui::MenuItem("Copy"))
				Debug::CopyGameObject();
				//CopyGameObject();

			if (ImGui::MenuItem("Delete"))
				DeleteObject();

			if (ImGui::MenuItem("Close"))
				m_spViewObj.reset();

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}

//==========================================================
// �I�𒆃I�u�W�F�N�g�̏��\��
//==========================================================
void CInspector::DispDebugSelectObject()
{
	if (!m_spViewObj.lock()) return;

	m_isDeleted = false;
	// �\���I�u�W�F�N�g�̃X�e�[�^�X�\��
	m_spViewObj.lock()->ImGuiDebug();
	ImGui::Separator();
	m_spViewObj.lock()->GetTransform()->ImGuiDebug();

	//--- ���߰���
	int cnt = 0;
	auto components = m_spViewObj.lock()->GetComponentList();
	if (m_isDrawInfo.size() != components.size())
	{
		m_isDrawInfo.resize(components.size());
	}

	cnt = 0;
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
	for (auto com : components)
	{
		if (ImGui::CollapsingHeader(com->GetName().c_str(), m_isDrawInfo[cnt]))
		{
			// ���߰��Ẵf�o�b�O�\��
			com->ImGuiDebug();

			if (ImGui::Button("Component Delete"))
			{
				if(!dynamic_cast<CTransform*>(com.get()))
					m_spViewObj.lock()->RemoveComponent(com);
			}
			if (m_isDeleted) break;
		}
		++cnt;
	}
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

}

//==========================================================
// ���߰��Ēǉ��E�B���h�E
// �N���b�N�Œǉ�
//==========================================================
void CInspector::AddComponentWindow()
{
	if (!m_bIsAddComponent)return;

	ImGui::SetNextWindowPos(ImVec2(1000, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_Once);
	std::vector<std::string> componentName = CComponentFactory::GetNameList();
	static std::string serchWord;
	
	if (ImGui::Begin("AddComponent", &m_bIsAddComponent, ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_::ImGuiWindowFlags_HorizontalScrollbar))
	{
		//--- ��������
		char input[56] = "\0";
		strcpy(input, serchWord.c_str());
		ImGui::Text("Serch Component");
		ImGui::SameLine();
		if (ImGui::InputText("##tag", input, 56))
			serchWord = input;

		//--- �I�u�W�F�N�g�ɃR���|�[�l���g��ǉ�
		// �\��
		for (std::string str : componentName)
		{
			if (!serchWord.empty())
			{
				// �����Ώۂł͂Ȃ�
				if (str.find(serchWord) == std::string::npos)
					continue;
			}

			// ���͂���
			if (ImGui::Button(str.c_str()))
			{
				// �R���|�[�l���g��ۑ����Ă���׽�ɒǉ����Ă��炤
				CComponentFactory::ObjSetComponent(*m_spViewObj.lock(), str);

				// �Ō���ɒǉ�����Ă���R���|�[�l���g���擾
				auto com = m_spViewObj.lock()->GetComponentList().back();
				// �������������s���B�ʏ�ł���ΕK�v�Ȃ����A���C�����[�v���̒ǉ��̂��߁A�����ōs��Ȃ���΂Ȃ�Ȃ�
				com->Init();

				m_isDrawInfo.push_back(false);
				m_bIsAddComponent = false;
				break;
			}
		}
		ImGui::End();
	}

}

#endif // !BUILD_MODE
