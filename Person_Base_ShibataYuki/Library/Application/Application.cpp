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
	const char*		WINDOW_TITLE = "SPEL"; //"�^�C�g��";
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
	// ���ƍ���������
	//CScreen::SetSize(1280.0f, 960.0f);

	// �E�C���h�E�쐬
	CWindow* window = &CWindow::Get();
	//window->RegisterClass(h_Instance, WINDOW_CLASS_NAME, CS_OWNDC);
	window->RegisterClass(h_Instance, WINDOW_CLASS_NAME, CS_CLASSDC);
	
	// COM������
	if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED))) 
	{
		MessageBox(NULL, _T("COM�̏������Ɏ��s���܂����B"), _T("error"), MB_OK);
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

	// �ǂݍ��݂��K�v�ȃV�X�e���̊֐����Ăяo��

	return true;
}

void Application::Uninit()
{
	// �������݂��K�v�ȃV�X�e���̊֐����Ăяo��

	// �V�X�e���̉��
	// ��ɉ�����������̂�����΂������ɌĂяo��
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
	
	// �Q�[���̕ϐ��錾
	CGameApp* gameApp = new CGameApp();	

	// �ǂݍ��݂��K�v�ȃV�X�e���̊֐����Ăяo��
	//CFuncManager::Get();

	// �Q�[���̏���������
	gameApp->Init(*this);

	// �^�C������������
	CFps::Get().Init();

	// �������[�v
	while (window->ExecMessage()) 
	{
		// fps�X�V
		CFps::Get().Update();

		// �Œ莞�ԍX�V
		if (!CFps::Get().IsFixedUpdate())
			gameApp->FixedUpdate(*this);

		// ��莞�Ԃ̍X�V
		if (!CFps::Get().IsUpdate())
			continue;

		// �Ăяo�����΍X�V
		gameApp->InputUpdate();

		// �֐��|�C���^�̍X�V
		CFuncManager::Get().Update();

		// �ްэX�V
		gameApp->Update(*this);

		// �ްѕ`��
		gameApp->Draw(*this);
	}

	CFps::Get().Uninit();

	// �Q�[���̏I������
	gameApp->Uninit(*this);
	delete gameApp;
	
	// �I�����b�Z�[�W
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