//=========================================================
// [imGuiManager.cpp]
// 作成:2022/07/10
//---------------------
// ImGui管理ｸﾗｽ
//=========================================================

//--- インクルード部
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
// コンストラクタ
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
// デストラクタ
//==========================================================
ImGuiManager::~ImGuiManager()
{
	Uninit();
}

//==========================================================
// オーナーのデバッグフラグ確認
//==========================================================
bool ImGuiManager::CheckPlayMode()
{
	return m_bEditFlg;
}

//==========================================================
// 初期化
//==========================================================
HRESULT ImGuiManager::Init(HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* context)
{
	HRESULT hr = S_OK;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	//iniを生成しないように
	io.IniFilename = NULL;
	//io.IniFilename = FORDER_DIR(Data / imgui.ini);
	
	// docking設定
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	//日本語フォントに対応
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
	
	//--- デバッグカメラの生成
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
// ImGuiの終了処理
//==========================================================
void ImGuiManager::Uninit()
{
	//--- 破棄 or 解放
	m_pDebugCamera.lock()->GetOwner(0).reset();
	m_pDebugCamera.reset();

	m_pInspector.reset();
	m_pHierarchy.reset();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

//==========================================================
// ImGuiの更新処理
// ImGuiの表示は基本ここで呼び出す
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

	//--- imGuiの更新処理
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();

	//--- 初期化
	m_eHover = EMouseHovered::HOVERED_NONE;
	//--- ポーズ処理
	Pause();

	//--- デバッグ実行
	m_pInspector->Update(this);
	
	m_pHierarchy->Update(this);
	
	// 現在シーン取得
	SceneManager::CScene* scene = CSceneManager::Get()->GetActiveScene();

	// 左表示
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	auto screenSize = CScreen::GetSize();
	screenSize.x *= 0.2f;
	screenSize.y *= 0.4f;
	ImGui::SetNextWindowPos(ImVec2(0, CScreen::GetHeight() - screenSize.y), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(screenSize.x, screenSize.y), ImGuiCond_Once);
	ImGui::Begin(u8"ステータス", &m_bEditFlg, ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar);
	ImGui::BeginTabBar("tabs", ImGuiTabBarFlags_None);

	//--- 状態取得セット
	HoverStateSet();

	//--- 基本
	if (ImGui::BeginTabItem("Base"))
	{
		ImGui::Text("Edit Mode -> LShift + [HOME]");

		// シーン名表示
		ImGui::Text(u8"現在のシーン名 : %s", scene->GetSceneName().c_str());
		ImGui::Text(u8"オブジェクト数 : %d", CSceneManager::Get()->GetActiveScene()->GetObjManager()->GetList().size());
		Debug::PrefabSelect();
		
		// フレームレート表示
		ImGui::EndTabItem();	// とじる
	}

	//--- SceneManager表示
	if (ImGui::BeginTabItem("SceneManager"))
	{
		CSceneManager::Get()->ImGuiDebug();
		// 変更されている可能性があるため再取得
		scene = CSceneManager::Get()->GetActiveScene();
		ImGui::EndTabItem();	// とじる
	}
	
	//--- FPS情報
	if (ImGui::BeginTabItem("FPS"))
	{
		CFps::Get()->ImGuiDebug();
		ImGui::EndTabItem();	// とじる
	}
	
	//--- 描画の確認
	if (ImGui::BeginTabItem("DrawSystem"))
	{
		CSceneManager::Get()->GetDrawSystem()->ImGuiDebug();
		ImGui::EndTabItem();	// とじる
	}
	
	//--- NavMesh
	if (ImGui::BeginTabItem("NavMesh"))
	{
		CSceneManager::Get()->GetNavMesh()->ImGuiDebug();		
		ImGui::EndTabItem();	// とじる
	}

	//--- ギズモ表示
	if(m_bGridDisp)
		m_pGizmo->ViewGrid(*CCamera::GetMain());
	if (auto selectObj = m_pInspector->GetSelectObject().lock(); selectObj)
	{
		m_pGizmo->ViewGizmo(this, *CCamera::GetMain(), selectObj->GetTransform());
		if (ImGui::BeginTabItem("Gizmo"))
		{
			m_pGizmo->EditTransform(this);
			ImGui::EndTabItem();// とじる
		}
	}

	//--- マウス状態確認
	if (ImGui::IsDragDropPayloadBeingAccepted())
		UpHover(EMouseHovered::HOVERED_DRAG);

	//--- ｶﾒﾗ操作
	// いずれかが操作中か確認
	const int e = EMouseHovered::HOVERED_WINDOW | EMouseHovered::HOVERED_GIZMO | EMouseHovered::HOVERED_ITEM | EMouseHovered::HOVERED_DRAG;
	if (!this->IsHover(EMouseHovered(e)))
	{
		if (m_pDebugCamera.lock())
			m_pDebugCamera.lock()->Update();
	}
	
	//--- ImGui機能表示
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
		ImGui::EndTabItem();// とじる
	}

	//--- ログ表示
	//DispLog();
	
	//--- タブを閉じる
	ImGui::EndTabBar();

	ImGui::End();
	ImGui::PopStyleColor();

	//--- デバッグカメラマトリックス更新
	if (m_pDebugCamera.lock())
		m_pDebugCamera.lock()->LateUpdate();

	return;
}

//==========================================================
// ImGuiの描画処理
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
// ポーズ処理
//==========================================================
void ImGuiManager::Pause()
{
	if (!m_bEditFlg)
		return;

	//--- 画面位置を外部から取得できるようにする
	ImGui::SetNextWindowPos(ImVec2((float)CScreen::GetWidth()*0.75f, (float)CScreen::GetHeight()*0.85f), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(320, 120), ImGuiCond_Once);
	
	ImGui::Begin(u8"Pause / 1Frame Step", &m_bPause);

	ImGui::Text("stop -> LShift + [L]");
	ImGui::Text("step -> LShift + [O]");

	// デバッグポーズ処理
	if (ImGui::Button("STOP") || CInput::GetKeyTrigger(VK_L) && CInput::GetKeyPress(VK_LSHIFT))
	{
		m_bPause ^= true;
	}

	//--- 1フレーム進める時
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
		// 停止を一時的にやめる
		if (m_bOneFlame)
		{
			m_bPause = false;
		}
	}
	//--- 1フレーム進んで戻ってきたとき
	else if (m_bOneFlame && !m_bPause)
	{
		m_bOneFlame = false;
		m_bPause = true;
	}


	ImGui::End();
}

//==========================================================
// ログの表示
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
// マウスの状態設定
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
// ギズモ表示
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