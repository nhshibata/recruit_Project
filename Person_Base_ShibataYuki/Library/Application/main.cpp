
// <Summary>
// ������g���̂ƁA
// </Summary>

// *@ ������g���̂͂ǂ��炪�����̂�

// �g���t�H�[�}�b�g?�ɂ���ĈقȂ�?

// �x���}�~
#define _CRT_SECURE_NO_WARNINGS

//#define _CRTDBG_MAP_ALLOC
#define NOMINMAX
//--- �C���N���[�h��
//#include <Windows.h>
#include <Application/main.h>
#include <Application/Application.h>

#define CONSOLE	0

//#if MODE
#if !MODE

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	using namespace MySpace::System;
	
	UNREFERENCED_PARAMETER(hPrevInstance);	// ���g�p�錾
	UNREFERENCED_PARAMETER(lpCmdLine);		// ���g�p�錾

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	// �R���\�[��
	AllocConsole();
	SetConsoleTitle("������");

	//--- �A�v���P�[�V�����̊J�n
	Application* Appli = &Application::Get();
	Appli->Init(hInstance);
	
	//--- �E�B���h�E�\��
	ShowWindow(Appli->GetHWnd(), nCmdShow);
	UpdateWindow(Appli->GetHWnd());

	//--- �X�V
	Appli->MainLoop();

	//--- �I��
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
	//UNREFERENCED_PARAMETER(hPrevInstance);	// ���g�p�錾
	//UNREFERENCED_PARAMETER(lpCmdLine);		// ���g�p�錾

	// ���������[�N���o
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	// �R���\�[��
#if CONSOLE
	AllocConsole();
	SetConsoleTitle("������");
#endif // CONSOLE

	//--- �A�v���P�[�V�����̊J�n
	HRESULT hr = S_OK;

	//--- ����
	Application::Create();

	Application* Appli = &Application::Get();
	hr = Appli->Init(hInstance);

	//--- �E�B���h�E�\��
	ShowWindow(Appli->GetHWnd(), nCmdShow);
	UpdateWindow(Appli->GetHWnd());
	return hr;
}
void ShutDown()
{
	Application* Appli = &Application::Get();

	//--- �I��
	Appli->Uninit();

	//--- �j��
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

	//--- �X�V
	Appli->MainLoop();
}

#endif


