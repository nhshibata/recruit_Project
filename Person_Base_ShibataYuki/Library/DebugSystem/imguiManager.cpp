#include <DebugSystem/imguiManager.h>
#include <DebugSystem/hierarchy.h>
#include <DebugSystem/inspector.h>

#ifdef BUILD_MODE

#include <CoreSystem/Input/input.h>

#include <Application/Application.h>
#include <Application/screen.h>
#include <CoreSystem/Time/fps.h>
#include <CoreSystem/FuncPtr/funcPtrManager.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/Manager/sceneManager.h>
#include <GameSystem/Manager/gameObjectManager.h>
#include <GraphicsSystem/Manager/imageResourceManager.h>

#include <GameSystem/Component/Camera/camera.h>
#include <GameSystem/Component/Camera/debugCamera.h>

using namespace MySpace::System;
using namespace MySpace::Debug;
using namespace MySpace::Game;
using namespace MySpace::Graphics;
using namespace MySpace::SceneManager;

// �R���X�g���N�^
ImGuiManager::ImGuiManager()
{
	m_bPause = false;
	m_bOneFlame = false;
	m_flg = false;
}
// �I�[�i�[�̃f�o�b�O�t���O�m�F
bool ImGuiManager::CheckPlayMode()
{
	return m_flg;
}
// ������
void ImGuiManager::Init(HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* context)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	//ini�𐶐����Ȃ��悤��
	io.IniFilename = NULL;
	//���{��t�H���g�ɑΉ�
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\meiryo.ttc", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(device, context);
	ImGui::StyleColorsDark();

	m_pInspector = std::make_shared<CInspector>();
	m_pHierarchy = std::make_shared<CHierachy>();
}
// ImGui�̏I������
void ImGuiManager::Uninit()
{
	m_pInspector->Uninit();
	m_pHierarchy->Uninit();
	m_pInspector.reset();
	m_pHierarchy.reset();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
// ImGui�̍X�V����
void ImGuiManager::Update() 
{
	// ON/OFF
	if (CInput::GetKeyTrigger(VK_I))
	{
		m_flg ^= true;
		if (!m_pDebugCamera.lock())
		{
			m_pDebugCamera = CCamera::GetMain()->AddComponent<CDebugCamera>();
		}
		else
		{
			m_pDebugCamera.lock()->SetActive(m_flg);
		}
	}
	if (!m_flg)
		return;

	if (m_pDebugCamera.lock())
		m_pDebugCamera.lock()->Update();

	//imGui�̍X�V����
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// �|�[�Y
	Pause();

	// �f�o�b�O���s
	m_pInspector->Update();
	m_pHierarchy->Update();

	// ���݃V�[���擾
	SceneManager::CScene* scene = CSceneManager::Get().GetActiveScene();
	
	// ����\��
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(20, 300), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_Once);
	ImGui::Begin(u8"�X�e�[�^�X", &m_flg, ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar);

	CSceneManager::Get().ImguiDebug();
	// �ύX����Ă���\��
	scene = CSceneManager::Get().GetActiveScene();

	// �t���[�����[�g��\��
	ImGui::Text(u8"���݂�FPS : %.1f FPS", ImGui::GetIO().Framerate);
	ImGui::Text(u8"���݂�DeltaTime : %.5f", CFps::Get().DeltaTime());

	// �V�[�����̕\��
	ImGui::Text(u8"���݂̃V�[���� : %s", scene->GetSceneName().c_str());
	ImGui::Text(u8"�I�u�W�F�N�g�� : %d", CSceneManager::Get().GetActiveScene()->GetObjManager()->GetList().size());
	
	scene->GetDrawManager()->ImGuiDebug();

	// ���O�\��
	DispLog();
	
	// �֐��|�C���^�\��
	//CFuncManager::Get().ImGuiDebug();
	ImGui::End();
	ImGui::PopStyleColor();

	return;
}

// ImGui�̕`��X�V����
void ImGuiManager::Render()
{
	if (!m_flg)
		return;

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
// �|�[�Y�̏���
void ImGuiManager::Pause()
{
	ImGui::SetNextWindowPos(ImVec2((float)CScreen::GetWidth() / 2 - 300 / 2, (float)0));   //��ʈʒu���O������擾�ł���悤�ɂ���
	
	if (!m_flg)
		return;
	ImGui::SetNextWindowSize(ImVec2(420, 120), ImGuiCond_Once);
	ImGui::Begin("Pause", &m_bPause);

	ImGui::Text(u8"stop[L]");
	ImGui::SameLine();
	ImGui::Text(u8"step[O]");
	ImGui::SameLine();

	// �f�o�b�O�|�[�Y�̏���
	if (ImGui::Button("STOP") || CInput::GetKeyTrigger(VK_L))
	{
		m_bPause ^= true;
	}

	// 1�t���[���i�߂�����
	if (m_bPause)
	{
		ImGui::SameLine();
		if (ImGui::Button("STEP") || CInput::GetKeyTrigger(VK_O))
		{
			if (!m_bOneFlame)
			{
				m_bOneFlame = true;
			}
		}
		// ��~���ꎞ�I�ɂ�߂�
		if (m_bOneFlame)
		{
			m_bPause = false;
		}
	}
	// 1�t���[���߂��Ă����Ƃ��̏���
	else if (m_bOneFlame && !m_bPause)
	{
		m_bOneFlame = false;
		m_bPause = true;
	}

	ImGui::End();
}
void ImGuiManager::DispLog()
{
	/*DebugLog("aaa");
	DebugLog("bbb");
*/
	//ImGui::SetNextWindowPos(ImVec2(120, 60), ImGuiCond_::ImGuiCond_Once);
	//ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_::ImGuiCond_Once);
	//ImGui::Begin(u8"Log");
	ImGui::Text(u8"Log");
	ImGui::SameLine();
	auto it = m_debugMap.begin();
	for (; it != m_debugMap.end(); ++it)
	{
		auto str = (*it).first;
		auto cstr = (*it).first.c_str();
		ImGui::Text(u8"%d", (*it).second);
		ImGui::SameLine();
		ImGui::Text(u8"%s", cstr);
	}
	//ImGui::End();
}

#endif