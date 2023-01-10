//=========================================================
//作成:2022/05/24
// ゲーム実行ｸﾗｽ
//=========================================================

//--- インクルード部
#define NOMINMAX

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
//#include <GraphicsSystem/Manager/imageResourceManager.h>
//#include <GraphicsSystem/Manager/FontTexture.h>
#include <GraphicsSystem/Manager/modelManager.h>
#include <GraphicsSystem/Manager/effectManager.h>
//#include <GraphicsSystem/Manager/shaderManager.h>

#include <DebugSystem/imguiManager.h>
#include <DebugSystem/typeSaveManager.h>

#include <CoreSystem/Input/input.h>
#include <CoreSystem/Sound/Sound.h>
#include <CoreSystem/FuncPtr/funcPtrManager.h>

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
void CGameApp::Init(Application* app)
{
	HWND hWnd = Application::Get()->GetHWnd();

	//--- 変数宣言
	auto pDevice = app->GetDevice();
	auto pDC = app->GetDeviceContext();

	//--- 入力初期化
	CInput::Init();
	CGamePad::Init();
	Mouse::Initialize();
	Keyboad::Update();

	//--- メッシュ
	CMesh::InitShader();

	// モデル
	CAssimpModel::InitShader(pDevice);

	//--- ポリゴン
	CPolygon::Init(pDevice);

	//--- アセット
	// 素材全般所持ｸﾗｽ
	auto pAssets = app->AddSystem<CAssetsManager>();
	pAssets->Init(app);

	// 音初期化
	CSound::Init();

	//--- シーンの生成
	auto sceneMgr = &CSceneManager::Get();
	sceneMgr->Init();
	

#ifdef BUILD_MODE
	//--- imGuiの初期化処理
	auto imgui = app->AddSystem<ImGuiManager>();
	if (!imgui)
	{
		int a = 0;
	}
	imgui->Init(hWnd, pDevice, pDC);
#endif // BUILD_MODE

	app->AddSystem<CTweenManager>();

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
		auto all = CSceneManager::Get().GetAllScene();
		for (auto & scene : all)
		{
			scene->GetObjManager()->UpdateInDebug();
		}
		return;
	}
#endif // DEBUG

	CSceneManager::Get().UpdateScene();

	// Tweenの更新(順番検討)
	app->GetSystem<CTweenManager>()->Update();

	app->GetSystem<CAssetsManager>()->Update();
}

//==========================================================
// 一定時間の更新
//==========================================================
void CGameApp::FixedUpdate(Application* app)const
{
	CSceneManager::Get().FixedUpdateScene();
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
	//--- 描画先設定
	BeginRender(app);	// 描画準備

#ifdef BUILD_MODE
	auto imgui = app->GetSystem<ImGuiManager>();

	if (imgui->IsSceneRender())
	{
		imgui->SceneRenderClear();
		imgui->SceneRender();
	}
	else
	{
		auto pDX = app->GetSystem<CDXDevice>();
		pDX->SwitchRender(nullptr, nullptr);
	}

	// オブジェクトが存在しないとき
	if (CCamera::GetMain() && CLight::GetMain())
	{
		// スカイドーム描画
		CCamera::GetMain()->DrawSkyDome();
		
		// シーンの描画
		CSceneManager::Get().DrawScene();

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
		CSceneManager::Get().DrawScene();

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
	pDC->ClearRenderTargetView(pDX->GetRenderTargetView(), ClearColor);
	pDC->ClearDepthStencilView(pDX->GetDepthStencilView(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	/*ID3D11RenderTargetView* pViews[] = {
		pDX->GetRenderTargetView()
	};*/
	//pDC->OMSetRenderTargets(1, pViews, nullptr);
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