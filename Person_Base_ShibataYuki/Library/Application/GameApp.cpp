//=========================================================
//作成:2022/05/24
// ゲーム実行ｸﾗｽ
//=========================================================

//--- インクルード部
#define NOMINMAX

#include <Application/GameApp.h>
#include <Application/Application.h>
#include <Application/screen.h>
#include <Application/systemManager.h>

#include <GameSystem/Manager/sceneManager.h>
#include <GameSystem/Manager/gameObjectManager.h>
#include <GameSystem/Scene/titleScene.h>
#include <GameSystem/Scene/buildScene.h>
#include <GameSystem/Component/Camera/camera.h>
#include <GameSystem/Component/Transform/Tween.h>
#include <GameSystem/GameObject/tag.h>

#include <GraphicsSystem/DirectX/DXDevice.h>
#include <GraphicsSystem/Render/polygon.h>
#include <GraphicsSystem/Render/mesh.h>
#include <GraphicsSystem/Manager/imageResourceManager.h>
#include <GraphicsSystem/Manager/FontTexture.h>
#include <GraphicsSystem/Manager/modelManager.h>
#include <GraphicsSystem/Manager/effectManager.h>
#include <GraphicsSystem/Manager/shaderManager.h>

#include <DebugSystem/imguiManager.h>
#include <DebugSystem/typeSaveManager.h>
#include <CoreSystem/system.h>
#include <CoreSystem/Input/input.h>
#include <CoreSystem/Sound/Sound.h>
#include <CoreSystem/FuncPtr/funcPtrManager.h>

using namespace MySpace::System;
using namespace MySpace::Graphics;
using namespace MySpace::Game;
using namespace MySpace::Debug;
using namespace MySpace::SceneManager;

// コンストラクタ
CGameApp::CGameApp()
{
}
// デストラクタ
CGameApp::~CGameApp()
{
}
// 初期化
void CGameApp::Init(Application& pApp)
{
	HWND hWnd = pApp.GetHWnd();

	// デバイスの初期化
	auto pDX = &CDXDevice::Get();
	pDX->Init(hWnd, (unsigned int)CScreen::GetWidth(), (unsigned int)CScreen::GetHeight());
	auto pD = pDX->GetDevice();
	auto pDc = pDX->GetDeviceContext();

#ifdef BUILD_MODE
	// imGuiの初期化処理
	auto imgui = &ImGuiManager::Get();
	imgui->Init(hWnd, pD, pDc);
#endif // BUILD_MODE
	
	// 入力の初期化
	CInput::Init();
	CGamePad::Init();
	Mouse::Initialize();
	Keyboad::Update();

	// メッシュ
	CMesh::InitShader();

	// 平行光源の初期化
	// ポストエフェクト?
	// スカイドーム

	// シーンの生成
	auto scene = &CSceneManager::Get();
	//CSceneManager::Get()->Init();
	//auto scene = CSceneManager::Get()->CreateNewScene<CScene>("Title");
	//CSceneManager::Get()->CreateNewScene<CScene>("Title");
	scene->Init();
	scene->CreateNewScene<CScene>("Title");

	// モデル
	auto model = &CModelManager::Get();
	model->Init();

	// ポリゴン
	CPolygon::Init(pD);

	// ｶﾒﾗの生成後
	auto effect = &CEffekseer::Get();
	effect->Init(pD, pDc);

	// フォント
	auto font = &CFontTexture::Get();
	font->Init();

	// シェーダー
	//CShaderManager::Get()->Init();

	// 音初期化
	CSound::Init();

	// 解放処理のため格納しておく
	/*pApp.MainSystem.Get()->SetSystem(ESystems::TAG, &CTagName::Get());
	pApp.MainSystem.Get()->SetSystem(ESystems::TWEEN, &CTweenManager::Get());
	pApp.MainSystem.Get()->SetSystem(ESystems::IMAGE, &CImageResourceManager::Get());
	pApp.MainSystem.Get()->SetSystem(ESystems::TYPE_SAVE, &CTypeSaveManager::Get());
	pApp.MainSystem.Get()->SetSystem(ESystems::FUNC_PTR, &CFuncManager::Get());*/

}
// 解放処理
void CGameApp::Uninit(Application& pApp)const
{
	// 音終了
	CSound::Fin();

	// 描画関係
	CMesh::FinShader();
	CPolygon::Fin();

	CFontTexture::Get().Uninit();

	CModelManager::Get().Uninit();

	CSceneManager::Get().Uninit();
	//CShaderManager::Get()->Uninit();
	
	// 入力
	CInput::Fin();
	CGamePad::Uninit();
}
// 通常更新
void CGameApp::Update(Application& pApp)const
{
	// 音更新
	CSound::Update();

#ifdef BUILD_MODE	// ImGui
	auto imgui = &ImGuiManager::Get();
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

	CSceneManager::Get().Update();
	CEffekseer::Get().Update();
}
// 定期更新
void CGameApp::FixedUpdate(Application& pApp)const
{
	// 一定時間の更新
	CSceneManager::Get().FixedUpdate();
}
// 入力更新
void CGameApp::InputUpdate()
{
	CInput::Update();
	CGamePad::Update();		// ｹﾞｰﾑ入力	
	Mouse::Update();

	Keyboad::Update();
}
// 描画
void CGameApp::Draw(Application& pApp)
{
	// 描画準備
	BeginRender(pApp);

	// ライトオブジェクトが存在しないとき
	if (CCamera::GetMain())
	{
		// シーンの描画
		CSceneManager::Get().Draw();

		// effect
		CSceneManager::Get().Draw();
	}

#ifdef BUILD_MODE
	// ImGuiの描画
	ImGuiManager::Get().Render();
#endif // BUILD_MODE

	// 描画後更新
	EndRender(pApp);
}
// *@描画前
void CGameApp::BeginRender(Application& pApp)
{
	float ClearColor[4] = { 0.117647f, 0.254902f, 0.352941f, 1.0f };
	auto pDX = &CDXDevice::Get();
	ID3D11DeviceContext* pDC = pDX->GetDeviceContext();
	pDC->ClearRenderTargetView(pDX->GetRenderTargetView(), ClearColor);
	pDC->ClearDepthStencilView(pDX->GetDepthStencilView(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	ID3D11RenderTargetView* pViews[] = {
		pDX->GetRenderTargetView()
	};
	pDC->OMSetRenderTargets(1, pViews, nullptr);

	CCamera::GetMain()->DrawSkyDome();
}
// *@描画後
void CGameApp::EndRender(Application& pApp)
{
	// 描画後更新
	auto pDX = &CDXDevice::Get();
	pDX->SetCullMode((int)ECullMode::CULLMODE_NONE);
	pDX->GetSwapChain()->Present(0, 0);
}