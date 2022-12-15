
// <Summary>
// これを使うのと、
// </Summary>

// *@ これを使うのはどちらがいいのか

// 使うフォーマット?によって異なる?

// 警告抑止
#define _CRT_SECURE_NO_WARNINGS

//#define _CRTDBG_MAP_ALLOC
#define NOMINMAX
//--- インクルード部
//#include <Windows.h>
#include <Application/main.h>
#include <Application/Application.h>

#define CONSOLE	0

//#if MODE
#if !MODE

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	using namespace MySpace::System;
	
	UNREFERENCED_PARAMETER(hPrevInstance);	// 未使用宣言
	UNREFERENCED_PARAMETER(lpCmdLine);		// 未使用宣言

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	// コンソール
	AllocConsole();
	SetConsoleTitle("初期化");

	//--- アプリケーションの開始
	Application* Appli = &Application::Get();
	Appli->Init(hInstance);
	
	//--- ウィンドウ表示
	ShowWindow(Appli->GetHWnd(), nCmdShow);
	UpdateWindow(Appli->GetHWnd());

	//--- 更新
	Appli->MainLoop();

	//--- 終了
	Appli->Uninit();

	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, hStdout);
	
	FreeConsole();
	_CrtDumpMemoryLeaks();

	return 0;
}
#else

//#pragma comment(lib, "CoreSystem")
//#pragma comment(lib, "DebugSystem")
//#pragma comment(lib, "GameSystem")
//#pragma comment(lib, "GraphicsSystem")

HRESULT StartUp(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//UNREFERENCED_PARAMETER(hPrevInstance);	// 未使用宣言
	//UNREFERENCED_PARAMETER(lpCmdLine);		// 未使用宣言

	// メモリリーク検出
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	// コンソール
#if CONSOLE
	AllocConsole();
	SetConsoleTitle("初期化");
#endif // CONSOLE

	//--- アプリケーションの開始
	HRESULT hr = S_OK;

	//--- 生成
	Application::Create();

	Application* Appli = &Application::Get();
	hr = Appli->Init(hInstance);

	//--- ウィンドウ表示
	ShowWindow(Appli->GetHWnd(), nCmdShow);
	UpdateWindow(Appli->GetHWnd());
	return hr;
}
void ShutDown()
{
	Application* Appli = &Application::Get();

	//--- 終了
	Appli->Uninit();

	//--- 破棄
	Application::Destroy();

#if CONSOLE
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, hStdout);
	_CrtDumpMemoryLeaks();

	FreeConsole();

	_CrtDumpMemoryLeaks();
#endif //!CONSOLE

}
void MainLoop()
{
	Application* Appli = &Application::Get();

	//--- 更新
	Appli->MainLoop();
}

#endif


