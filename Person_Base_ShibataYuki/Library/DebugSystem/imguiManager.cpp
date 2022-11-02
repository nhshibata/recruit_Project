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

// コンストラクタ
ImGuiManager::ImGuiManager()
{
	m_bPause = false;
	m_bOneFlame = false;
	m_flg = false;
}
// オーナーのデバッグフラグ確認
bool ImGuiManager::CheckPlayMode()
{
	return m_flg;
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

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(device, context);
	ImGui::StyleColorsDark();

	m_pInspector = std::make_shared<CInspector>();
	m_pHierarchy = std::make_shared<CHierachy>();
}
// ImGuiの終了処理
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
// ImGuiの更新処理
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

	//imGuiの更新処理
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

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
	ImGui::Begin(u8"ステータス", &m_flg, ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar);

	CSceneManager::Get().ImguiDebug();
	// 変更されている可能性
	scene = CSceneManager::Get().GetActiveScene();

	// フレームレートを表示
	ImGui::Text(u8"現在のFPS : %.1f FPS", ImGui::GetIO().Framerate);
	ImGui::Text(u8"現在のDeltaTime : %.5f", CFps::Get().DeltaTime());

	// シーン名の表示
	ImGui::Text(u8"現在のシーン名 : %s", scene->GetSceneName().c_str());
	ImGui::Text(u8"オブジェクト数 : %d", CSceneManager::Get().GetActiveScene()->GetObjManager()->GetList().size());
	
	scene->GetDrawManager()->ImGuiDebug();

	// ログ表示
	DispLog();
	
	// 関数ポインタ表示
	//CFuncManager::Get().ImGuiDebug();
	ImGui::End();
	ImGui::PopStyleColor();

	return;
}

// ImGuiの描画更新処理
void ImGuiManager::Render()
{
	if (!m_flg)
		return;

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
// ポーズの処理
void ImGuiManager::Pause()
{
	ImGui::SetNextWindowPos(ImVec2((float)CScreen::GetWidth() / 2 - 300 / 2, (float)0));   //画面位置を外部から取得できるようにする
	
	if (!m_flg)
		return;
	ImGui::SetNextWindowSize(ImVec2(420, 120), ImGuiCond_Once);
	ImGui::Begin("Pause", &m_bPause);

	ImGui::Text(u8"stop[L]");
	ImGui::SameLine();
	ImGui::Text(u8"step[O]");
	ImGui::SameLine();

	// デバッグポーズの処理
	if (ImGui::Button("STOP") || CInput::GetKeyTrigger(VK_L))
	{
		m_bPause ^= true;
	}

	// 1フレーム進めたい時
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