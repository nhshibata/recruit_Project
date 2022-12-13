//==========================================================



//--- �C���N���[�h��
#include <DebugSystem/imguiManager.h>
#include <DebugSystem/hierarchy.h>
#include <DebugSystem/inspector.h>
#include <DebugSystem/imGuiSceneGizmo.h>

#ifdef BUILD_MODE

#include <CoreSystem/Input/input.h>

#include <Application/Application.h>
#include <Application/screen.h>
#include <CoreSystem/Time/fps.h>
#include <CoreSystem/FuncPtr/funcPtrManager.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/Manager/sceneManager.h>
#include <GameSystem/Manager/gameObjectManager.h>
#include <GameSystem/Manager/drawSystem.h>
#include <GraphicsSystem/Manager/imageResourceManager.h>
#include <GraphicsSystem/DirectX/renderTarget.h>
#include <GraphicsSystem/DirectX/depthStencil.h>

#include <GameSystem/Component/Camera/camera.h>
#include <GameSystem/Component/Camera/debugCamera.h>
#include <AISystem/Nav/navMeshBake.h>

#pragma region NAME_SPACE
using namespace MySpace::System;
using namespace MySpace::Debug;
using namespace MySpace::Game;
using namespace MySpace::Graphics;
using namespace MySpace::SceneManager;
#pragma endregion

// �R���X�g���N�^
ImGuiManager::ImGuiManager()
{
	m_bPause = false;
	m_bOneFlame = false;
	m_bEditFlg = false;
	m_bSceneRender = false;
}
// �I�[�i�[�̃f�o�b�O�t���O�m�F
bool ImGuiManager::CheckPlayMode()
{
	return m_bEditFlg;
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
	{
		io.DisplaySize.x = CScreen::GetWidth();
		io.DisplaySize.y = CScreen::GetHeight();
	}

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(device, context);
	ImGui::StyleColorsDark();

	m_pInspector = std::make_shared<CInspector>();
	m_pHierarchy = std::make_shared<CHierachy>();
	m_pGizmo = std::make_shared<CMyGizmo>();
	m_pGizmo->Init();
	m_pDS = std::make_shared<CDepthStencil>();
	m_pRT = std::make_shared<CRenderTarget>();
	
	// �f�o�b�O�J�����̐���
	{
		m_pDebugObj = std::make_shared<CGameObject>();
		CGameObject::CreateDebugObject(m_pDebugObj);
		m_pDebugCamera = m_pDebugObj->AddComponent<CDebugCamera>();
		m_pDebugCamera.lock()->Init();
	}
}
// ImGui�̏I������
void ImGuiManager::Uninit()
{
	m_pDebugCamera.lock()->GetOwner(0).reset();
	m_pDebugCamera.reset();

	m_pInspector->Uninit();
	m_pHierarchy->Uninit();
	m_pInspector.reset();
	m_pHierarchy.reset();
	m_pDS.reset();
	m_pRT.reset();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
// ImGui�̍X�V����
void ImGuiManager::Update() 
{
	if (CInput::GetKeyTrigger(VK_P))
	{
		m_bSceneRender ^= true;
	}
	// ON/OFF
	if (CInput::GetKeyTrigger(VK_I))
	{
		m_bEditFlg ^= true;
		m_pDebugCamera.lock()->ResumeCamera(m_bEditFlg);
	}
	if (!m_bEditFlg)
		return;

	//imGui�̍X�V����
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();

	ImGuiManager::Get().DownHover(ImGuiManager::EIsHovered::HOVERED_WINDOW);

	// �|�[�Y
	Pause();

	// �f�o�b�O���s
	m_pInspector->Update();
	m_pHierarchy->Update();
	
	// ���݃V�[���擾
	SceneManager::CScene* scene = CSceneManager::Get().GetActiveScene();
	
	//--- SceneView�\��
	if (m_bSceneRender)
	{
		ImGui::SetNextWindowPos(ImVec2(CScreen::GetWidth()*0.15f, 0), ImGuiCond_::ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(CScreen::GetWidth()*0.65f, CScreen::GetHeight()*0.65f), ImGuiCond_::ImGuiCond_Always);
		ImGui::Begin("SceneView");
		ImGui::Image(m_pRT->GetSRV(), ImVec2(CScreen::GetWidth()*0.65f, CScreen::GetHeight()*0.65f));
		ImGui::End();
	}

	// ���\��
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(0, 300), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_Once);
	ImGui::Begin(u8"�X�e�[�^�X", &m_bEditFlg, ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar);
	ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None);
	
	//--- ��{
	if (ImGui::BeginTabItem("Base"))
	{
		ImGui::Checkbox("RenderSwitch", &m_bSceneRender);

		// �V�[�����\��
		ImGui::Text(u8"���݂̃V�[���� : %s", scene->GetSceneName().c_str());
		ImGui::Text(u8"�I�u�W�F�N�g�� : %d", CSceneManager::Get().GetActiveScene()->GetObjManager()->GetList().size());

		// �t���[�����[�g�\��
		ImGui::Text(u8"���݂�FPS : %.1f FPS", ImGui::GetIO().Framerate);
		ImGui::EndTabItem();	// �Ƃ���
	}

	//--- SceneManager�\��
	if (ImGui::BeginTabItem("SceneManager"))
	{
		CSceneManager::Get().ImGuiDebug();
		// �ύX����Ă���\�������邽�ߍĎ擾
		scene = CSceneManager::Get().GetActiveScene();
		ImGui::EndTabItem();	// �Ƃ���
	}
	
	//--- FPS���
	if (ImGui::BeginTabItem("FPS"))
	{
		CFps::Get().ImGuiDebug();
		ImGui::EndTabItem();	// �Ƃ���
	}
	
	//--- �`��̊m�F
	if (ImGui::BeginTabItem("DrawSystem"))
	{
		CSceneManager::Get().GetDrawSystem()->ImGuiDebug();
		ImGui::EndTabItem();	// �Ƃ���
	}
	
	//--- NavMesh
	if (ImGui::BeginTabItem("NavMesh"))
	{
		CSceneManager::Get().GetNavMesh()->ImGuiDebug();		
		ImGui::EndTabItem();	// �Ƃ���
	}

	//--- �M�Y���\��
	if (auto selectObj = m_pInspector->GetSelectObject().lock(); selectObj)
	{
		if (ImGui::BeginTabItem("Gizmo"))
		{
			m_pGizmo->EditTransform(*CCamera::GetMain(), selectObj->GetTransform());
			ImGui::EndTabItem();// �Ƃ���
		}
	}

	//--- �}�E�X��Ԋm�F
	if(ImGui::IsDragDropPayloadBeingAccepted())
		UpHover(EIsHovered::HOVERED_DRAG);

	if (ImGui::IsAnyItemHovered()) 
	{
		UpHover(EIsHovered::HOVERED_ITEM);
	}
	else
		DownHover(EIsHovered::HOVERED_ITEM);

	//--- ��ב���
	const int e = ImGuiManager::EIsHovered::HOVERED_WINDOW | ImGuiManager::EIsHovered::HOVERED_GIZMO;
	if (!ImGuiManager::Get().IsHover(EIsHovered(e)))
	//if (ImGuiManager::Get().GetHover() == EIsHovered::HOVERED_NONE)
	{
		if (m_pDebugCamera.lock())
			m_pDebugCamera.lock()->Update();
	}
	int res = 0;
	for (int cnt = 1; cnt < sizeof(EIsHovered) ; cnt++)
	{
		res += m_eHover & (1 << cnt) ? 1 : 0;
		res *= 10;
	}
	ImGui::Text("Hover->%d", res);

	//--- ���O�\��
	DispLog();
	
	// �֐��|�C���^�\��

	//--- �^�u�����
	ImGui::EndTabBar();

	ImGui::End();
	ImGui::PopStyleColor();

	//--- �f�o�b�O�J�����}�g���b�N�X�X�V
	if (m_pDebugCamera.lock())
		m_pDebugCamera.lock()->LateUpdate();

	return;
}

// ImGui�̕`��X�V����
void ImGuiManager::Render()
{
	if (!m_bEditFlg)
		return;

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_::ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

// �|�[�Y�̏���
void ImGuiManager::Pause()
{
	if (!m_bEditFlg)
		return;

	ImGui::SetNextWindowPos(ImVec2((float)CScreen::GetWidth() / 2 - 300 / 2, (float)0));   //��ʈʒu���O������擾�ł���悤�ɂ���
	ImGui::SetNextWindowSize(ImVec2(420, 120), ImGuiCond_Once);
	ImGui::Begin(u8"Pause", &m_bPause);

	ImGui::Text(u8"stop[L]");
	ImGui::SameLine();
	ImGui::Text(u8"step[O]");
	ImGui::SameLine();

	// �f�o�b�O�|�[�Y�̏���
	if (ImGui::Button(u8"STOP") || CInput::GetKeyTrigger(VK_L))
	{
		m_bPause ^= true;
	}

	// 1�t���[���i�߂�����
	if (m_bPause)
	{
		ImGui::SameLine();
		if (ImGui::Button(u8"STEP") || CInput::GetKeyTrigger(VK_O))
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

ID3D11RenderTargetView* ImGuiManager::GetRTV()
{
	return m_pRT->GetView();
}
ID3D11DepthStencilView* ImGuiManager::GetDSV()
{
	return m_pDS->GetView();
}
void ImGuiManager::SceneRender()
{
	auto pDX = &CDXDevice::Get();
	//--- �`���̕ύX
	pDX->SwitchRender(m_pRT->GetView(), m_pDS->GetView());
}
void ImGuiManager::SceneRenderClear()
{
	float ClearColor[4] = { 0.117647f, 0.254902f, 0.352941f, 1.0f };

	//--- �������Z�b�g
	m_pRT->Clear(ClearColor);
	m_pDS->Clear();
}

#endif