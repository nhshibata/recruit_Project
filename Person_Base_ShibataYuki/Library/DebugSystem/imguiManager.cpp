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

// コンストラクタ
ImGuiManager::ImGuiManager()
{
	m_bPause = false;
	m_bOneFlame = false;
	m_bEditFlg = false;
}
// オーナーのデバッグフラグ確認
bool ImGuiManager::CheckPlayMode()
{
	return m_bEditFlg;
}
// 初期化
void ImGuiManager::Init(HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* context)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	//iniを生成しないように
	io.IniFilename = NULL;
	//日本語フォントに対応
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
	
	// デバッグカメラの生成
	{
		m_pDebugObj = std::make_shared<CGameObject>();
		CGameObject::CreateDebugObject(m_pDebugObj);
		m_pDebugCamera = m_pDebugObj->AddComponent<CDebugCamera>();
		m_pDebugCamera.lock()->Init();
	}
}
// ImGuiの終了処理
void ImGuiManager::Uninit()
{
	m_pDebugCamera.lock()->GetOwner(0).reset();
	m_pDebugCamera.reset();

	m_pInspector->Uninit();
	m_pHierarchy->Uninit();
	m_pInspector.reset();
	m_pHierarchy.reset();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
// ImGuiの更新処理
void ImGuiManager::Update() 
{
	// ON/OFF
	if (CInput::GetKeyTrigger(VK_I))
	{
		m_bEditFlg ^= true;
		m_pDebugCamera.lock()->ResumeCamera(m_bEditFlg);
	}
	if (!m_bEditFlg)
		return;

	//imGuiの更新処理
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();

	ImGuiManager::Get().DownHover(ImGuiManager::EIsHovered::HOVERED_WINDOW);

	// ポーズ
	Pause();

	// デバッグ実行
	m_pInspector->Update();
	m_pHierarchy->Update();
	
	// 現在シーン取得
	SceneManager::CScene* scene = CSceneManager::Get().GetActiveScene();
	
	// 左上表示
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(20, 300), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_Once);
	ImGui::Begin(u8"ステータス", &m_bEditFlg, ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar);

	if (auto selectObj = m_pInspector->GetSelectObject().lock(); selectObj)
		m_pGizmo->EditTransform(*CCamera::GetMain(), selectObj->GetTransform());

	// SceneManager表示
	CSceneManager::Get().ImguiDebug();
	// 変更されている可能性があるため再取得
	scene = CSceneManager::Get().GetActiveScene();

	// シーン名表示
	ImGui::Text(u8"現在のシーン名 : %s", scene->GetSceneName().c_str());
	ImGui::Text(u8"オブジェクト数 : %d", CSceneManager::Get().GetActiveScene()->GetObjManager()->GetList().size());

	// フレームレート表示
	ImGui::Text(u8"現在のFPS : %.1f FPS", ImGui::GetIO().Framerate);
	CFps::Get().ImGuiDebug();	
	
	// 描画の確認
	CSceneManager::Get().GetDrawSystem()->ImGuiDebug();

	// NavMesh
	CSceneManager::Get().GetNavMesh()->ImGuiDebug();

	//--- マウス状態確認
	if(ImGui::IsDragDropPayloadBeingAccepted())
		UpHover(EIsHovered::HOVERED_DRAG);

	if (ImGui::IsAnyItemHovered()) 
	{
		UpHover(EIsHovered::HOVERED_ITEM);
	}
	else
		DownHover(EIsHovered::HOVERED_ITEM);

	//--- ｶﾒﾗ操作
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

	// スカイドーム描画
	if (m_pDebugCamera.lock())
		m_pDebugCamera.lock()->LateUpdate();
	//m_pDebugCamera.lock()->DrawSkyDome();

	// ログ表示
	DispLog();
	
	// 関数ポインタ表示

	ImGui::End();
	ImGui::PopStyleColor();

	return;
}

// ImGuiの描画更新処理
void ImGuiManager::Render()
{
	if (!m_bEditFlg)
		return;

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
// ポーズの処理
void ImGuiManager::Pause()
{
	if (!m_bEditFlg)
		return;

	ImGui::SetNextWindowPos(ImVec2((float)CScreen::GetWidth() / 2 - 300 / 2, (float)0));   //画面位置を外部から取得できるようにする
	ImGui::SetNextWindowSize(ImVec2(420, 120), ImGuiCond_Once);
	ImGui::Begin(u8"Pause", &m_bPause);

	ImGui::Text(u8"stop[L]");
	ImGui::SameLine();
	ImGui::Text(u8"step[O]");
	ImGui::SameLine();

	// デバッグポーズの処理
	if (ImGui::Button(u8"STOP") || CInput::GetKeyTrigger(VK_L))
	{
		m_bPause ^= true;
	}

	// 1フレーム進めたい時
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
		// 停止を一時的にやめる
		if (m_bOneFlame)
		{
			m_bPause = false;
		}
	}
	// 1フレーム戻ってきたときの処理
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