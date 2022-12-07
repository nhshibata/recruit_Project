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
#include <Application/systemManager.h>
#include <CoreSystem/system.h>
#include <CoreSystem/Time/fps.h>
#include <CoreSystem/FuncPtr/funcPtrManager.h>
#include <GraphicsSystem/DirectX/DXDevice.h>

using namespace MySpace::System;
using namespace MySpace::Graphics;

#define CLASS_NAME		_T("win32app")

namespace
{
	const char*		WINDOW_TITLE = "SPEL"; //"タイトル";
	const char*		WINDOW_CLASS_NAME = CLASS_NAME;

	const uint32_t	WINDOW_STYLE_WINDOWED = (WS_VISIBLE | WS_CAPTION | WS_SYSMENU);
	const uint32_t	WINDOW_EX_STYLE_WINDOWED = (0);
	const uint32_t	WINDOW_STYLE_FULL_SCREEN = (WS_VISIBLE | WS_POPUP);
	const uint32_t	WINDOW_EX_STYLE_FULL_SCREEN = (0);

	uint32_t		SYSTEM_WIDTH = 0;
	uint32_t		SYSTEM_HEIGHT = 0;
}

bool Application::Init(HINSTANCE h_Instance)
{
	// 幅と高さ初期化
	//CScreen::SetSize(1280.0f, 960.0f);

	// ウインドウ作成
	CWindow* window = &CWindow::Get();
	//window->RegisterClass(h_Instance, WINDOW_CLASS_NAME, CS_OWNDC);
	window->RegisterClass(h_Instance, WINDOW_CLASS_NAME, CS_CLASSDC);
	
	// COM初期化
	if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED))) 
	{
		MessageBox(NULL, _T("COMの初期化に失敗しました。"), _T("error"), MB_OK);
		return false;
	}

	window->SetWindow(h_Instance,
		WINDOW_STYLE_WINDOWED,
		WINDOW_EX_STYLE_WINDOWED,
		NULL,
		static_cast<long>(CScreen::GetWidth() + SYSTEM_WIDTH),
		static_cast<long>(CScreen::GetHeight() + SYSTEM_HEIGHT),
		WINDOW_CLASS_NAME,
		WINDOW_TITLE,
		true);

	m_hWnd = window->GetHWND();
	m_hInst = h_Instance;

	// 読み込みが必要なシステムの関数を呼び出す

	return true;
}

void Application::Uninit()
{
	// 書き込みが必要なシステムの関数を呼び出す

	// システムの解放
	// 先に解放したいものがあればこれより先に呼び出す
	//auto all = MainSystem.Get()->GetAll();
	//for (auto it = all.begin(); it != all.end(); ++it)
	//{
	//	CBase* app = reinterpret_cast<CBase*>(*it);
	//	if (app)
	//		app->Uninit();
	//}

	//CWindow::Get().Close(WINDOW_CLASS_NAME, m_hInst);
	UnregisterClass(WINDOW_CLASS_NAME, m_hInst);

	return;
}

unsigned long Application::MainLoop()
{
	CWindow* window = &CWindow::Get();
	
	// ゲームの変数宣言
	CGameApp* gameApp = new CGameApp();	

	// 読み込みが必要なシステムの関数を呼び出す
	//CFuncManager::Get();

	// ゲームの初期化処理
	gameApp->Init(*this);

	// タイム初期化処理
	CFps::Get().Init();

	// 無限ループ
	while (window->ExecMessage()) 
	{
		// fps更新
		CFps::Get().Update();

		// 固定時間更新
		if (!CFps::Get().IsFixedUpdate())
			gameApp->FixedUpdate(*this);

		// 一定時間の更新
		if (!CFps::Get().IsUpdate())
			continue;

		// 呼び出されれば更新
		gameApp->InputUpdate();

		// 関数ポインタの更新
		CFuncManager::Get().Update();

		// ｹﾞｰﾑ更新
		gameApp->Update(*this);

		// ｹﾞｰﾑ描画
		gameApp->Draw(*this);
	}

	CFps::Get().Uninit();

	// ゲームの終了処理
	gameApp->Uninit(*this);
	delete gameApp;
	
	// 終了メッセージ
	return window->GetMessage();
}
ID3D11Device* Application::GetDevice()
{
	return CDXDevice::Get().GetDevice();
}
ID3D11DeviceContext* Application::GetDeviceContext()
{
	return CDXDevice::Get().GetDeviceContext();
}