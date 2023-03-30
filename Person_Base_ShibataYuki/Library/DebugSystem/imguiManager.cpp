//=========================================================
// [imGuiManager.cpp]
// �쐬:2022/07/10
//---------------------
// ImGui�Ǘ��׽
//=========================================================

//--- �C���N���[�h��
#include <ImGui/imgui_impl_win32.h>
#include <ImGui/imgui_impl_dx11.h>
#include <ImGui/imgui_internal.h>
#include <ImGui/imstb_rectpack.h>
#include <ImGui/imstb_textedit.h>
#include <ImGui/imstb_truetype.h>

#include <DebugSystem/imguiManager.h>
#include <DebugSystem/hierarchy.h>
#include <DebugSystem/inspector.h>
#include <DebugSystem/imGuiSceneGizmo.h>
#include <DebugSystem/imGuiContextMenu.h>

#ifdef BUILD_MODE

#include <CoreSystem/Input/input.h>

#include <Application/Application.h>
#include <Application/screen.h>
#include <CoreSystem/Time/fps.h>
#include <CoreSystem/FuncPtr/funcPtrManager.h>

#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/Component/Camera/camera.h>
#include <GameSystem/Component/Camera/debugCamera.h>
#include <GameSystem/Manager/sceneManager.h>
#include <GameSystem/Manager/gameObjectManager.h>
#include <GameSystem/Manager/drawSystem.h>

#include <GraphicsSystem/Manager/imageResourceManager.h>
#include <GraphicsSystem/DirectX/renderTarget.h>

#include <AISystem/Nav/navMeshBake.h>

using namespace MySpace::System;
using namespace MySpace::Debug;
using namespace MySpace::Game;
using namespace MySpace::Graphics;
using namespace MySpace::SceneManager;

//==========================================================
// �R���X�g���N�^
//==========================================================
ImGuiManager::ImGuiManager()
{
	m_bPlayMode = EPlayMode::Release;
	m_eHover = EMouseHovered::HOVERED_NONE;
	m_bPause = false;
	m_bOneFlame = false;
	m_bEditFlg = true;
	m_bGridDisp = false;

}

//==========================================================
// �f�X�g���N�^
//==========================================================
ImGuiManager::~ImGuiManager()
{
	Uninit();
}

//==========================================================
// �I�[�i�[�̃f�o�b�O�t���O�m�F
//==========================================================
bool ImGuiManager::CheckPlayMode()
{
	return m_bEditFlg;
}

//==========================================================
// ������
//==========================================================
HRESULT ImGuiManager::Init(HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* context)
{
	HRESULT hr = S_OK;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	//ini�𐶐����Ȃ��悤��
	io.IniFilename = NULL;
	//io.IniFilename = FORDER_DIR(Data / imgui.ini);
	
	// docking�ݒ�
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	//���{��t�H���g�ɑΉ�
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\meiryo.ttc", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
	{
		io.DisplaySize.x = CScreen::GetWidth();
		io.DisplaySize.y = CScreen::GetHeight();
	}

	if (!ImGui_ImplWin32_Init(hWnd))
		return S_FALSE;
	if(!ImGui_ImplDX11_Init(device, context))
		return S_FALSE;
	ImGui::StyleColorsDark();

	m_pInspector = std::make_shared<CInspector>();
	m_pHierarchy = std::make_shared<CHierachy>();
	m_pGizmo = std::make_shared<CMyGizmo>();
	m_pGizmo->Init();
	
	//--- �f�o�b�O�J�����̐���
	if(1)
	{	
		m_bEditFlg = true;
		m_pDebugObj = std::make_shared<CGameObject>();
		m_pDebugObj = CGameObject::CreateDebugObject(m_pDebugObj);
		m_pDebugCamera = m_pDebugObj->AddComponent<CDebugCamera>();
		m_pDebugCamera.lock()->Init();
		m_pDebugCamera.lock()->ResumeCamera(m_bEditFlg);
		m_pDebugObj->GetTransform()->SetScale({ 10, 10, 10 });
		m_pDebugObj->GetTransform()->Update();
	}
	else
	{
		m_bEditFlg = false;
	}
	return hr;
}

//==========================================================
// ImGui�̏I������
//==========================================================
void ImGuiManager::Uninit()
{
	//--- �j�� or ���
	m_pDebugCamera.lock()->GetOwner(0).reset();
	m_pDebugCamera.reset();

	m_pInspector.reset();
	m_pHierarchy.reset();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

//==========================================================
// ImGui�̍X�V����
// ImGui�̕\���͊�{�����ŌĂяo��
//==========================================================
void ImGuiManager::Update() 
{
	
	// ON/OFF
	if (CInput::GetKeyTrigger(VK_HOME) && CInput::GetKeyPress(VK_LSHIFT))
	{
		m_bEditFlg ^= true;
		if(m_pDebugCamera.lock())
			m_pDebugCamera.lock()->ResumeCamera(m_bEditFlg);
	}
	if (!m_bEditFlg)
		return;

	//--- imGui�̍X�V����
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();

	//--- ������
	m_eHover = EMouseHovered::HOVERED_NONE;
	//--- �|�[�Y����
	Pause();

	//--- �f�o�b�O���s
	m_pInspector->Update(this);
	
	m_pHierarchy->Update(this);
	
	// ���݃V�[���擾
	SceneManager::CScene* scene = CSceneManager::Get()->GetActiveScene();

	// ���\��
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	auto screenSize = CScreen::GetSize();
	screenSize.x *= 0.2f;
	screenSize.y *= 0.4f;
	ImGui::SetNextWindowPos(ImVec2(0, CScreen::GetHeight() - screenSize.y), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(screenSize.x, screenSize.y), ImGuiCond_Once);
	ImGui::Begin(u8"�X�e�[�^�X", &m_bEditFlg, ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar);
	ImGui::BeginTabBar("tabs", ImGuiTabBarFlags_None);

	//--- ��Ԏ擾�Z�b�g
	HoverStateSet();

	//--- ��{
	if (ImGui::BeginTabItem("Base"))
	{
		ImGui::Text("Edit Mode -> LShift + [HOME]");

		// �V�[�����\��
		ImGui::Text(u8"���݂̃V�[���� : %s", scene->GetSceneName().c_str());
		ImGui::Text(u8"�I�u�W�F�N�g�� : %d", CSceneManager::Get()->GetActiveScene()->GetObjManager()->GetList().size());
		Debug::PrefabSelect();
		
		// �t���[�����[�g�\��
		ImGui::EndTabItem();	// �Ƃ���
	}

	//--- SceneManager�\��
	if (ImGui::BeginTabItem("SceneManager"))
	{
		CSceneManager::Get()->ImGuiDebug();
		// �ύX����Ă���\�������邽�ߍĎ擾
		scene = CSceneManager::Get()->GetActiveScene();
		ImGui::EndTabItem();	// �Ƃ���
	}
	
	//--- FPS���
	if (ImGui::BeginTabItem("FPS"))
	{
		CFps::Get()->ImGuiDebug();
		ImGui::EndTabItem();	// �Ƃ���
	}
	
	//--- �`��̊m�F
	if (ImGui::BeginTabItem("DrawSystem"))
	{
		CSceneManager::Get()->GetDrawSystem()->ImGuiDebug();
		ImGui::EndTabItem();	// �Ƃ���
	}
	
	//--- NavMesh
	if (ImGui::BeginTabItem("NavMesh"))
	{
		CSceneManager::Get()->GetNavMesh()->ImGuiDebug();		
		ImGui::EndTabItem();	// �Ƃ���
	}

	//--- �M�Y���\��
	if(m_bGridDisp)
		m_pGizmo->ViewGrid(*CCamera::GetMain());
	if (auto selectObj = m_pInspector->GetSelectObject().lock(); selectObj)
	{
		m_pGizmo->ViewGizmo(this, *CCamera::GetMain(), selectObj->GetTransform());
		if (ImGui::BeginTabItem("Gizmo"))
		{
			m_pGizmo->EditTransform(this);
			ImGui::EndTabItem();// �Ƃ���
		}
	}

	//--- �}�E�X��Ԋm�F
	if (ImGui::IsDragDropPayloadBeingAccepted())
		UpHover(EMouseHovered::HOVERED_DRAG);

	//--- ��ב���
	// �����ꂩ�����쒆���m�F
	const int e = EMouseHovered::HOVERED_WINDOW | EMouseHovered::HOVERED_GIZMO | EMouseHovered::HOVERED_ITEM | EMouseHovered::HOVERED_DRAG;
	if (!this->IsHover(EMouseHovered(e)))
	{
		if (m_pDebugCamera.lock())
			m_pDebugCamera.lock()->Update();
	}
	
	//--- ImGui�@�\�\��
	if (ImGui::BeginTabItem("ImGui"))
	{
		ImGui::Checkbox("ImGui OFF", &m_bPause);

		ImGui::Checkbox("Grid", &m_bGridDisp);

		if(ImGui::Button("Camera Reset"))
			m_pDebugCamera.lock()->Reset();
		/*int res = 0;
		for (int cnt = 1; cnt < sizeof(EMouseHovered); cnt++)
		{
			res += m_eHover & (1 << cnt) ? 1 : 0;
			res *= 10;
		}
		ImGui::Text("Hover->%d", res);*/
		ImGui::EndTabItem();// �Ƃ���
	}

	//--- ���O�\��
	//DispLog();
	
	//--- �^�u�����
	ImGui::EndTabBar();

	ImGui::End();
	ImGui::PopStyleColor();

	//--- �f�o�b�O�J�����}�g���b�N�X�X�V
	if (m_pDebugCamera.lock())
		m_pDebugCamera.lock()->LateUpdate();

	return;
}

//==========================================================
// ImGui�̕`�揈��
//==========================================================
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

//==========================================================
// �|�[�Y����
//==========================================================
void ImGuiManager::Pause()
{
	if (!m_bEditFlg)
		return;

	//--- ��ʈʒu���O������擾�ł���悤�ɂ���
	ImGui::SetNextWindowPos(ImVec2((float)CScreen::GetWidth()*0.75f, (float)CScreen::GetHeight()*0.85f), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(320, 120), ImGuiCond_Once);
	
	ImGui::Begin(u8"Pause / 1Frame Step", &m_bPause);

	ImGui::Text("stop -> LShift + [L]");
	ImGui::Text("step -> LShift + [O]");

	// �f�o�b�O�|�[�Y����
	if (ImGui::Button("STOP") || CInput::GetKeyTrigger(VK_L) && CInput::GetKeyPress(VK_LSHIFT))
	{
		m_bPause ^= true;
	}

	//--- 1�t���[���i�߂鎞
	if (m_bPause)
	{
		ImGui::SameLine();
		if (ImGui::Button("STEP") || CInput::GetKeyTrigger(VK_O) && CInput::GetKeyPress(VK_LSHIFT))
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
	//--- 1�t���[���i��Ŗ߂��Ă����Ƃ�
	else if (m_bOneFlame && !m_bPause)
	{
		m_bOneFlame = false;
		m_bPause = true;
	}


	ImGui::End();
}

//==========================================================
// ���O�̕\��
//==========================================================
void ImGuiManager::DispLog()
{
	ImGui::Separator();
	ImGui::Text("Log");
	
	auto it = m_aDebugMap.begin();
	for (int cnt = 0; it != m_aDebugMap.end(); ++it, ++cnt)
	{
		auto str = (*it).first;
		auto cstr = (*it).first.c_str();
		
		ImGui::Text("%d", (*it).second);
		ImGui::SameLine();
		ImGui::Text(u8"%s", cstr);
	}

	//ImGui::End();
}

//==========================================================
// �}�E�X�̏�Ԑݒ�
//==========================================================
void ImGuiManager::HoverStateSet()
{

	if (ImGui::IsWindowHovered())
	{
		UpHover(ImGuiManager::EMouseHovered::HOVERED_WINDOW);
		//DebugLog("Window Hover");
	}

	if (ImGui::IsAnyItemHovered())
	{
		UpHover(ImGuiManager::EMouseHovered::HOVERED_ITEM);
		//DebugLog("Item Hover");
	}
	
	if (ImGui::IsDragDropPayloadBeingAccepted())
	{
		UpHover(ImGuiManager::EMouseHovered::HOVERED_DRAG);
		//DebugLog("Drag");
	}
	
	if (ImGui::IsAnyItemActive())
	{
		UpHover(ImGuiManager::EMouseHovered::HOVERED_ITEM);
		//DebugLog("Item Active");
	}

	if (ImGui::IsItemClicked())
	{
		//DebugLog("Item Click");
	}

	if (ImGui::IsItemEdited())
	{
		//DebugLog("Item Edit");
	}
}

//==========================================================
// �M�Y���\��
//==========================================================
void ImGuiManager::SceneGizmo()
{
	if (auto selectObj = m_pInspector->GetSelectObject().lock(); selectObj)
	{
		m_pGizmo->ViewGizmo(this, *CCamera::GetMain(), selectObj->GetTransform());
	}
}


//ImGuiManager* ImGuiManager::Get()
//{
//	static ImGuiManager instance;
//	return &instance;
//}

#endif