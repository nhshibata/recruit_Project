//=========================================================
// [Application.cpp]
// ゲーム実行ｸﾗｽ
//------------------------
// 作成:2022/05/24
// 更新:2022/11/12 FixedUpdate実装
// 
//=========================================================

//--- インクルード部
#define NOMINMAX
#include <Crtdbg.h>
#include <tchar.h>

#include <Application/Application.h>
#include <Application/GameApp.h>
#include <Application/window.h>
#include <Application/screen.h>
#include <CoreSystem/Time/fps.h>
#include <CoreSystem/FuncPtr/funcPtrManager.h>
#include <GraphicsSystem/DirectX/DXDevice.h>

//--- 定数定義
#define CLASS_NAME		_T("win32app")

using namespace MySpace::System;
using namespace MySpace::Graphics;

namespace
{
	const char*		WINDOW_TITLE = "SPELL"; //"タイトル";
	const char*		WINDOW_CLASS_NAME = CLASS_NAME;

	const uint32_t	WINDOW_STYLE_WINDOWED = (WS_VISIBLE | WS_CAPTION | WS_SYSMENU);
	const uint32_t	WINDOW_EX_STYLE_WINDOWED = (0);
	const uint32_t	WINDOW_STYLE_FULL_SCREEN = (WS_VISIBLE | WS_POPUP);
	const uint32_t	WINDOW_EX_STYLE_FULL_SCREEN = (0);

	uint32_t		SYSTEM_WIDTH = 0;
	uint32_t		SYSTEM_HEIGHT = 0;
}

//==========================================================
// 初期化
//==========================================================
bool Application::Init(HINSTANCE hInstance)
{
	// 幅と高さ初期化
	//CScreen::SetSize(1280.0f, 960.0f);

	//--- ウインドウ作成
	CWindow* window = AddSystem<CWindow>();
	window->RegisterClass(hInstance, WINDOW_CLASS_NAME, CS_CLASSDC);
	
	// COM初期化
	if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED))) 
	{
		MessageBox(NULL, _T("COMの初期化に失敗しました。"), _T("error"), MB_OK);
		return false;
	}

	window->SetWindow(hInstance,
		WINDOW_STYLE_WINDOWED,
		WINDOW_EX_STYLE_WINDOWED,
		NULL,
		static_cast<long>(CScreen::GetWidth() + SYSTEM_WIDTH),
		static_cast<long>(CScreen::GetHeight() + SYSTEM_HEIGHT),
		WINDOW_CLASS_NAME,
		WINDOW_TITLE,
		true);

	// 必要な変数を格納
	m_hWnd = window->GetHWND();
	m_hInst = hInstance;

	// 読み込みが必要なシステムの関数を呼び出す	
	AddSystem<CFuncManager>();

	//--- デバイスの初期化
	auto pDX = AddSystem<CDXDevice>();
	pDX->Init(m_hWnd, (unsigned int)CScreen::GetWidth(), (unsigned int)CScreen::GetHeight());
	m_pDevice = pDX->GetDevice();
	m_pDeviceContext = pDX->GetDeviceContext();

	return true;
}

//==========================================================
// 破棄
//==========================================================
void Application::Destroy()
{
	// 書き込みが必要なシステムの関数を呼び出す

	//CWindow::Get()->Close(WINDOW_CLASS_NAME, m_hInst);
	UnregisterClass(WINDOW_CLASS_NAME, m_hInst);
	
	//--- シングルトンの解放

	// 最後尾から解放
	for (auto rit = m_aSystems.rbegin(); rit != m_aSystems.rend();)
	{
		delete (*rit).second;
		++rit;
	}
	m_aSystems.clear();

	return;
}

//==========================================================
// ゲームループ
//==========================================================
unsigned long Application::MainLoop()
{
	CWindow* window = GetSystem<CWindow>();
	
	// ゲーム変数宣言
	CGameApp* gameApp = new CGameApp();	

	// ゲームの初期化処理
	gameApp->Init(this);

	//--- タイム初期化処理
	CFps::Get().Init();

	// 無限ループ
	while (window->ExecMessage()) 
	{
		//--- fps更新
		CFps::Get().Update();

		//--- 固定時間更新
		if (!CFps::Get().IsFixedUpdate())
			gameApp->FixedUpdate(this);

		//--- 通常更新確認
		if (!CFps::Get().IsUpdate())
			continue;

		//--- 入力更新
		gameApp->InputUpdate();

		//--- 関数ポインタ更新
		GetSystem<CFuncManager>()->Update();

		//--- ｹﾞｰﾑ更新
		gameApp->Run(this);

		//--- ｹﾞｰﾑ描画
		gameApp->Draw(this);
	}

	//--- 終了処理

	// タイム終了処理
	CFps::Get().Uninit();

	// ゲーム終了処理
	gameApp->Uninit(this);
	delete gameApp;
	
	// 終了メッセージ
	return window->GetMessage();
}

//==========================================================
// デバイス取得
//==========================================================
ID3D11Device* Application::GetDevice()
{
	return m_pDevice;
}

//==========================================================
// デバイス取得
//==========================================================
ID3D11DeviceContext* Application::GetDeviceContext()
{
	return m_pDeviceContext;
}