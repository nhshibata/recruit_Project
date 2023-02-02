//=========================================================
// [Application.cpp]
//---------------------------------------------------------
// �쐬:2022/05/24
// �X�V:2022/11/12 FixedUpdate����
// �X�V:2023/01/31 FixedUpdate���C���Bif(!IsFixedUpdate)�ɂȂ��Ă������߁B
//---------------------------------------------------------
// �Q�[�����s�׽
//=========================================================

//--- �C���N���[�h��
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

//--- �萔��`
#define CLASS_NAME		_T("win32app")

using namespace MySpace::System;
using namespace MySpace::Graphics;

namespace
{
	const char*		WINDOW_TITLE = "biscuit"; //"�^�C�g��";
	const char*		WINDOW_CLASS_NAME = CLASS_NAME;

	const uint32_t	WINDOW_STYLE_WINDOWED = (WS_VISIBLE | WS_CAPTION | WS_SYSMENU);
	const uint32_t	WINDOW_EX_STYLE_WINDOWED = (0);
	const uint32_t	WINDOW_STYLE_FULL_SCREEN = (WS_VISIBLE | WS_POPUP);
	const uint32_t	WINDOW_EX_STYLE_FULL_SCREEN = (0);

	uint32_t		SYSTEM_WIDTH = 0;
	uint32_t		SYSTEM_HEIGHT = 0;
}

//==========================================================
// ������
//==========================================================
Application::Application()
	:m_hInst(0),m_hWnd(0),m_pDevice(nullptr),m_pDeviceContext(nullptr)
{
}

//==========================================================
// ������
//==========================================================
Application::~Application()
{

}

//==========================================================
// ������
//==========================================================
bool Application::Init(HINSTANCE hInstance)
{
	HRESULT hr = S_OK;
	// ���ƍ���������
	//CScreen::SetSize(1280.0f, 960.0f);

	//--- process order 002
	//--- �E�C���h�E�쐬
	CWindow* window = new CWindow();
	AddSystem(window, typeid(CWindow).name());
	window->RegisterClass(hInstance, WINDOW_CLASS_NAME, CS_CLASSDC);
	
	// COM������
	if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED))) 
	{
		MessageBox(NULL, _T("COM�̏������Ɏ��s���܂����B"), _T("error"), MB_OK);
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

	// �K�v�ȕϐ����i�[
	m_hWnd = window->GetHWND();
	m_hInst = hInstance;

	// �ǂݍ��݂��K�v�ȃV�X�e���̊֐����Ăяo��	
	AddSystem(new CFuncManager(), typeid(CFuncManager).name());

	//--- process order 003
	//--- �f�o�C�X�̏�����
	auto pDX = new CDXDevice();
	AddSystem(pDX, typeid(CDXDevice).name());
	hr = pDX->Init(m_hWnd, (unsigned int)CScreen::GetWidth(), (unsigned int)CScreen::GetHeight());
	if (FAILED(hr))
	{
		MessageBox(NULL, _T("DirectX�̏������Ɏ��s���܂����B"), _T("error"), MB_OK);
		return false;
	}
	// �K�v�ȕϐ����i�[
	m_pDevice = pDX->GetDevice();
	m_pDeviceContext = pDX->GetDeviceContext();

	return hr == S_OK;
}

//==========================================================
// �j��
//==========================================================
void Application::Destroy()
{
	// �������݂��K�v�ȃV�X�e���̊֐����Ăяo��

	//--- �E�B���h�E���
	//CWindow::Get()->Close(WINDOW_CLASS_NAME, m_hInst);
	UnregisterClass(WINDOW_CLASS_NAME, m_hInst);
	
	//--- �V�X�e���������
	// �Ō�����珇�ɉ��
	for (auto rit = m_aSystems.rbegin(); rit != m_aSystems.rend();)
	{
		delete (*rit).second;
		++rit;
	}
	m_aSystems.clear();

	return;
}

//==========================================================
// �Q�[�����[�v
//==========================================================
unsigned long Application::MainLoop()
{
	//--- process order 004
	// �ϐ��錾
	CWindow* window = GetSystem<CWindow>();
	CGameApp* gameApp = new CGameApp();	

	//--- �Q�[���̏���������
	gameApp->Init(this);

	//--- �^�C������������
	CFps::Get()->Init();

	// �������[�v
	while (window->ExecMessage()) 
	{
		//--- fps�X�V
		CFps::Get()->Update();

		//--- �Œ莞�ԍX�V
		if (CFps::Get()->IsFixedUpdate())
			gameApp->FixedUpdate(this);

		//--- �ʏ�X�V�m�F
		if (!CFps::Get()->IsUpdate())
			continue;

		//--- ���͍X�V
		gameApp->InputUpdate();

		//--- �֐��|�C���^�X�V
		GetSystem<CFuncManager>()->Update();

		//--- �ްэX�V
		gameApp->Run(this);

		//--- �ްѕ`��
		gameApp->Draw(this);
	}

	//--- �I������

	// �^�C���I������
	CFps::Get()->Uninit();

	// �Q�[���I������
	gameApp->Uninit(this);
	delete gameApp;
	
	// �I�����b�Z�[�W
	return window->GetMessage();
}

//==========================================================
// �f�o�C�X�擾
//==========================================================
ID3D11Device* Application::GetDevice()
{
	return m_pDevice;
}

//==========================================================
// �f�o�C�X�擾
//==========================================================
ID3D11DeviceContext* Application::GetDeviceContext()
{
	return m_pDeviceContext;
}