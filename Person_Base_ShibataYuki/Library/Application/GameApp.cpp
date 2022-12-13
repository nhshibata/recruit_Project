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

	//--- デバイスの初期化
	CDXDevice::Create();
	auto pDX = &CDXDevice::Get();
	pDX->Init(hWnd, (unsigned int)CScreen::GetWidth(), (unsigned int)CScreen::GetHeight());
	//--- 変数宣言
	auto pD = pDX->GetDevice();
	auto pDc = pDX->GetDeviceContext();

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
	CTypeSaveManager::Create();

	//--- ゲーム要素
	CTagName::Create();
	CTweenManager::Create();

	// モデル
	CModelManager::Create();
	auto model = &CModelManager::Get();
	model->Init();

	//--- ポリゴン
	CPolygon::Init(pD);

	//--- ﾃｸｽﾁｬ
	CImageResourceManager::Create();

	//--- フォント
	CFontTexture::Create();
	auto font = &CFontTexture::Get();
	font->Init();

	//--- シーンの生成
	CSceneManager::Create();
	auto scene = &CSceneManager::Get();
	//CSceneManager::Get()->Init();
	//auto scene = CSceneManager::Get()->CreateNewScene<CScene>("Title");
	//CSceneManager::Get()->CreateNewScene<CScene>("Title");
	scene->Init();
	scene->CreateNewScene<CScene>("Title");

	//--- エフェクト生成・初期化(ｶﾒﾗの生成後)
	CEffekseer::Create();
	auto effect = &CEffekseer::Get();
	effect->Init(pD, pDc);

#ifdef BUILD_MODE
	// imGuiの初期化処理
	ImGuiManager::Create();
	auto imgui = &ImGuiManager::Get();
	imgui->Init(hWnd, pD, pDc);
#endif // BUILD_MODE

	// シェーダー
	//CShaderManager::Get()->Init();

	// 音初期化
	CSound::Init();

}
// 解放処理
void CGameApp::Uninit(Application& pApp)const
{

	_CrtDumpMemoryLeaks();

	// 音終了
	CSound::Fin();

	// 描画関係
	CMesh::FinShader();
	CPolygon::Fin();

	CFontTexture::Get().Uninit();

	CSceneManager::Get().Uninit();
	//CShaderManager::Get()->Uninit();
	
	CTypeSaveManager::Get().Uninit();

	// 入力
	CInput::Fin();
	CGamePad::Uninit();
	_CrtDumpMemoryLeaks();

	//--- シングルトン破棄
	CTweenManager::Destroy();
	CTagName::Destroy();
	CImageResourceManager::Create();
	CFontTexture::Destroy();
	CEffekseer::Destroy();
	CModelManager::Destroy();
	ImGuiManager::Destroy();
	CSceneManager::Destroy();
	CTypeSaveManager::Destroy();
	CDXDevice::Destroy();
	_CrtDumpMemoryLeaks();

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
	//--- 描画先設定
	BeginRender(pApp);	// 描画準備
	if (ImGuiManager::Get().IsSceneRender())
	{
		ImGuiManager::Get().SceneRenderClear();
		ImGuiManager::Get().SceneRender();
	}
	else
	{
		auto pDX = &CDXDevice::Get();
		pDX->SwitchRender(nullptr, nullptr);
	}
	
	CCamera::GetMain()->DrawSkyDome();

	// オブジェクトが存在しないとき
	if (CCamera::GetMain() && CLight::Get())
	{
		// シーンの描画
		CSceneManager::Get().Draw();

		// effect
		CEffekseer::Get().Draw();
	}

	//--- 描画先切替
	if (ImGuiManager::Get().IsSceneRender())
	{
		auto pDX = &CDXDevice::Get();
		pDX->SwitchRender(nullptr, nullptr);
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

	/*ID3D11RenderTargetView* pViews[] = {
		pDX->GetRenderTargetView()
	};*/
	//pDC->OMSetRenderTargets(1, pViews, nullptr);
}
// *@描画後
void CGameApp::EndRender(Application& pApp)
{
	auto pDX = &CDXDevice::Get();
	pDX->SetCullMode((int)ECullMode::CULLMODE_NONE);
	pDX->GetSwapChain()->Present(0, 0);
}