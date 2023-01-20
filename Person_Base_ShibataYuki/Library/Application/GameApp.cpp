//=========================================================
// [GameApp.cpp]
//---------------------------------------------------------
//作成:2022/05/24
//---------------------------------------------------------
// ゲーム実行ｸﾗｽ
//=========================================================

#define NOMINMAX

//--- インクルード部
#include <Application/GameApp.h>
#include <Application/Application.h>
#include <Application/screen.h>

#include <GameSystem/Manager/sceneManager.h>
#include <GameSystem/Manager/gameObjectManager.h>
#include <GameSystem/Component/Camera/camera.h>
#include <GameSystem/Component/Transform/Tween.h>
#include <GameSystem/GameObject/tag.h>

#include <GraphicsSystem/DirectX/DXDevice.h>
#include <GraphicsSystem/Render/polygon.h>
#include <GraphicsSystem/Render/mesh.h>
#include <GraphicsSystem/Manager/assetsManager.h>
#include <GraphicsSystem/Manager/modelManager.h>
#include <GraphicsSystem/Manager/effectManager.h>
//#include <GraphicsSystem/Manager/shaderManager.h>

#include <DebugSystem/imguiManager.h>
#include <DebugSystem/typeSaveManager.h>

#include <CoreSystem/Input/input.h>
#include <CoreSystem/Sound/Sound.h>
#include <CoreSystem/FuncPtr/funcPtrManager.h>

#include <tchar.h>

using namespace MySpace::System;
using namespace MySpace::Graphics;
using namespace MySpace::Game;
using namespace MySpace::Debug;
using namespace MySpace::SceneManager;


//==========================================================
// コンストラクタ
//==========================================================
CGameApp::CGameApp()
{
}

//==========================================================
// デストラクタ
//==========================================================
CGameApp::~CGameApp()
{
}

//==========================================================
// 初期化
//==========================================================
HRESULT CGameApp::Init(Application* app)
{
	HRESULT hr = S_OK;

	//--- 変数宣言
	auto pDevice = app->GetDevice();
	auto pDC = app->GetDeviceContext();

	//--- 入力初期化
	hr = CInput::Init();
	if(hr != S_OK)
		MessageBox(NULL, _T("CInputの初期化に失敗しました。"), _T("error"), MB_OK);
	CGamePad::Init();
	Mouse::Initialize();
	//Keyboad::();

	//--- メッシュ
	hr = CMesh::InitShader();
	if (hr != S_OK)
		MessageBox(NULL, _T("CMeshの初期化に失敗しました。"), _T("error"), MB_OK);
	//--- モデル
	hr = CAssimpModel::InitShader(pDevice) ? S_OK : S_FALSE;
	if (hr != S_OK)
		MessageBox(NULL, _T("CAssimpModelの初期化に失敗しました。"), _T("error"), MB_OK);
	//--- ポリゴン
	hr = CPolygon::InitShader(pDevice);
	if (hr != S_OK)
		MessageBox(NULL, _T("CPolygonの初期化に失敗しました。"), _T("error"), MB_OK);

	//--- アセット
	// 素材全般所持ｸﾗｽ
	auto pAssets = CAssetsManager::Get();
	app->AddSystem(pAssets, typeid(CAssetsManager).name());
	hr = pAssets->Init(app);
	if (hr != S_OK)
		MessageBox(NULL, _T("CAssetsManagerの初期化に失敗しました。"), _T("error"), MB_OK);

	// 音初期化
	CSound::Init();

	//--- シーンの生成
	{
		auto sceneMgr = CSceneManager::Get();
		hr = sceneMgr->Init();
		if (hr != S_OK)
			MessageBox(NULL, _T("CSceneManagerの初期化に失敗しました。"), _T("error"), MB_OK);
	}

	//--- Effekseer
	hr = pAssets->GetEffekseer()->Init(pDevice, pDC);
	if (hr != S_OK)
		MessageBox(NULL, _T("CEffekseerの初期化に失敗しました。"), _T("error"), MB_OK);

#ifdef BUILD_MODE
	//--- imGuiの初期化処理
	auto imgui = ImGuiManager::Get();
	app->AddSystem(imgui, typeid(ImGuiManager).name());
	hr = imgui->Init(Application::Get()->GetHWnd(), pDevice, pDC);
	if (hr != S_OK)
		MessageBox(NULL, _T("ImGuiの初期化に失敗しました。"), _T("error"), MB_OK);
#endif // BUILD_MODE

	//app->AddSystem<CTweenManager>();
	return hr;
}

//==========================================================
// 解放処理
//==========================================================
void CGameApp::Uninit(Application* app)const
{
	//_CrtDumpMemoryLeaks();

	// 音終了
	CSound::Fin();

	// Assimp用シェーダ終了処理
	CAssimpModel::UninitShader();

	// 描画関係
	CMesh::FinShader();
	CPolygon::Fin();

	CTypeSaveManager::Get()->Uninit();

	// 入力
	CInput::Fin();
	CGamePad::Uninit();
	_CrtDumpMemoryLeaks();

}

//==========================================================
// 通常更新
//==========================================================
void CGameApp::Run(Application* app)
{
	// 音更新
	CSound::Update();

#ifdef BUILD_MODE	// ImGui
	auto imgui = app->GetSystem<ImGuiManager>();
	imgui->Update();
	
	// デバッグ中の更新(GameObjectのdeleteとTransformの更新はないと不便)
	if (imgui->GetPause())
	{
		auto all = CSceneManager::Get()->GetAllScene();
		for (auto & scene : all)
		{
			scene->GetObjManager()->UpdateInDebug();
		}
		return;
	}
#endif // DEBUG

	//--- シーン更新
	CSceneManager::Get()->UpdateScene();

	// Tweenの更新(順番検討)
	//app->GetSystem<CTweenManager>()->Update();

	app->GetSystem<CAssetsManager>()->Update();
}

//==========================================================
// 一定時間の更新
//==========================================================
void CGameApp::FixedUpdate(Application* app)const
{
	CSceneManager::Get()->FixedUpdateScene();
}

//==========================================================
// 入力更新
//==========================================================
void CGameApp::InputUpdate()
{
	CInput::Update();
	CGamePad::Update();		// ｹﾞｰﾑ入力	
	Mouse::Update();

	Keyboad::Update();
}

//==========================================================
// 描画
//==========================================================
void CGameApp::Draw(Application* app)
{
	//--- 描画クリア
	BeginRender(app);

#ifdef BUILD_MODE
	auto imgui = app->GetSystem<ImGuiManager>();

	//--- 描画先変更
	if (imgui->IsSceneRender())
	{
		imgui->SceneRenderClear();
		imgui->SceneRender();
	}
	else
	{
		auto pDX = app->GetSystem<CDXDevice>();
		pDX->SwitchRender(pDX->GetRenderTargetView(), pDX->GetDepthStencilView());
	}

	// オブジェクトが存在しないとき
	if (CCamera::GetMain() && CLight::GetMain())
	{
		// スカイドーム描画
		CCamera::GetMain()->DrawSkyDome();
		
		// シーンの描画
		CSceneManager::Get()->DrawScene();

		// effect
		app->GetSystem<CAssetsManager>()->GetEffekseer()->Draw();
	}

	imgui->SceneGizmo();

	//--- 描画先切替
	if (imgui->IsSceneRender())
	{
		auto pDX = app->GetSystem<CDXDevice>();
		pDX->SwitchRender(nullptr, nullptr);
	}

	//--- ImGuiの描画
	imgui->Render();

#else
	// オブジェクトが存在しないとき
	if (CCamera::GetMain() && CLight::GetMain())
	{
		CCamera::GetMain()->DrawSkyDome();
		
		// シーンの描画
		CSceneManager::Get()->DrawScene();

		// effect
		app->GetSystem<CAssetsManager>()->GetEffekseer()->Draw();
	}
#endif // BUILD_MODE

	// 描画後更新
	EndRender(app);
}

//==========================================================
// 描画前
//==========================================================
void CGameApp::BeginRender(Application* app)
{
	float ClearColor[4] = { 0.117647f, 0.254902f, 0.352941f, 1.0f };
	auto pDX = app->GetSystem<CDXDevice>();
	ID3D11DeviceContext* pDC = pDX->GetDeviceContext();
	//--- ﾒｲﾝ画面クリア
	pDC->ClearRenderTargetView(pDX->GetRenderTargetView(), ClearColor);
	pDC->ClearDepthStencilView(pDX->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	/*ID3D11RenderTargetView* pViews[] = {
		pDX->GetRenderTargetView()
	};
	pDC->OMSetRenderTargets(1, pViews, pDX->GetDepthStencilView());*/
}

//==========================================================
// 描画後
//==========================================================
void CGameApp::EndRender(Application* app)
{
	auto pDX = app->GetSystem<CDXDevice>();
	pDX->SetCullMode((int)ECullMode::CULLMODE_NONE);
	pDX->GetSwapChain()->Present(0, 0);
}