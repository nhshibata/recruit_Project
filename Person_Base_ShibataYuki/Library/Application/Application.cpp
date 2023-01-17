//=========================================================
// [Application.cpp]
// �Q�[�����s�׽
//------------------------
// �쐬:2022/05/24
// �X�V:2022/11/12 FixedUpdate����
// 
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
	const char*		WINDOW_TITLE = "SPELL"; //"�^�C�g��";
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
bool Application::Init(HINSTANCE hInstance)
{
	// ���ƍ���������
	//CScreen::SetSize(1280.0f, 960.0f);

	//--- �E�C���h�E�쐬
	CWindow* window = AddSystem<CWindow>();
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
	AddSystem<CFuncManager>();

	//--- �f�o�C�X�̏�����
	auto pDX = AddSystem<CDXDevice>();
	pDX->Init(m_hWnd, (unsigned int)CScreen::GetWidth(), (unsigned int)CScreen::GetHeight());
	m_pDevice = pDX->GetDevice();
	m_pDeviceContext = pDX->GetDeviceContext();

	return true;
}

//==========================================================
// �j��
//==========================================================
void Application::Destroy()
{
	// �������݂��K�v�ȃV�X�e���̊֐����Ăяo��

	//CWindow::Get()->Close(WINDOW_CLASS_NAME, m_hInst);
	UnregisterClass(WINDOW_CLASS_NAME, m_hInst);
	
	//--- �V���O���g���̉��

	// �Ō��������
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
	CWindow* window = GetSystem<CWindow>();
	
	// �Q�[���ϐ��錾
	CGameApp* gameApp = new CGameApp();	

	// �Q�[���̏���������
	gameApp->Init(this);

	//--- �^�C������������
	CFps::Get().Init();

	// �������[�v
	while (window->ExecMessage()) 
	{
		//--- fps�X�V
		CFps::Get().Update();

		//--- �Œ莞�ԍX�V
		if (!CFps::Get().IsFixedUpdate())
			gameApp->FixedUpdate(this);

		//--- �ʏ�X�V�m�F
		if (!CFps::Get().IsUpdate())
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
	CFps::Get().Uninit();

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