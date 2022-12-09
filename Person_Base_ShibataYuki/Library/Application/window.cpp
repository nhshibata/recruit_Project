//=========================================================
// [window.cpp] 
// �쐬:2022/05/24
//---------------------------------------------------------
//=========================================================

#include <Application/window.h>
#include <CoreSystem/Input/mouse.h>
#include <ImGui/imgui.h>

using namespace MySpace;
using namespace MySpace::System;

//--- �萔��`

//--- �ÓI�����o�ϐ�

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT ImGui_WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lparam)
{
	
	// �R�[���o�b�N���ݒ肳��Ă��Ȃ���ΏI��
	ImGui_WndProc(hwnd, msg, wParam, lparam);
	
	// ���b�Z�[�W����
	switch (msg)
	{
		// �L�[����
	case WM_KEYDOWN:
		// GM
		switch (wParam) {
		case VK_ESCAPE:					// [ESC]�L�[�������ꂽ
			PostMessage(hwnd, WM_CLOSE, 0, 0);	// [x]�������ꂽ�悤�ɐU����
			DestroyWindow(hwnd);
			return DefWindowProc(hwnd, msg, wParam, lparam);
		}
		return DefWindowProc(hwnd, msg, wParam, lparam);

		// �L�[���
	case WM_KEYUP:
		return DefWindowProc(hwnd, msg, wParam, lparam);

		// �V�X�e���L�[����
	case WM_SYSKEYDOWN:
		return DefWindowProc(hwnd, msg, wParam, lparam);

		// �V�X�e���L�[���
	case WM_SYSKEYUP:
		return DefWindowProc(hwnd, msg, wParam, lparam);

		// �~
	case WM_CLOSE:
		PostQuitMessage(0);
		return DefWindowProc(hwnd, msg, wParam, lparam);

		// �E�C���h�E���
	case WM_DESTROY:
		PostQuitMessage(0);
		return DefWindowProc(hwnd, msg, wParam, lparam);

		// �ĕ`��
	case WM_PAINT:
		return DefWindowProc(hwnd, msg, wParam, lparam);

		//=========================================================
	case WM_LBUTTONDOWN:
	{
		SetCapture(hwnd);	// �J�[�\�����O�ɏo�Ă���������悤�ɐݒ�
		Input::Mouse::SetButtonDown(Input::Mouse::EButton::Left);
		break;
	}
	case WM_LBUTTONUP:
	{
		ReleaseCapture();	// �J�[�\�����O�ɏo�Ă��������Ȃ��悤�ɐݒ�
		Input::Mouse::SetButtonUp(Input::Mouse::EButton::Left);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		SetCapture(hwnd);	// �J�[�\�����O�ɏo�Ă���������悤�ɐݒ�
		Input::Mouse::SetButtonDown(Input::Mouse::EButton::Right);
		break;
	}
	case WM_RBUTTONUP:
	{
		ReleaseCapture();	// �J�[�\�����O�ɏo�Ă��������Ȃ��悤�ɐݒ�
		Input::Mouse::SetButtonUp(Input::Mouse::EButton::Right);
		break;
	}
	case WM_MBUTTONDOWN:
	{
		SetCapture(hwnd);	// �J�[�\�����O�ɏo�Ă���������悤�ɐݒ�
		Input::Mouse::SetButtonDown(Input::Mouse::EButton::Middle);
		break;
	}
	case WM_MBUTTONUP:
	{
		ReleaseCapture();	// �J�[�\�����O�ɏo�Ă��������Ȃ��悤�ɐݒ�
		Input::Mouse::SetButtonUp(Input::Mouse::EButton::Middle);
		break;
	}
	case WM_MOUSEWHEEL:
	{
		//DWORD fwKeys = GET_KEYSTATE_WPARAM(wParam);	// �����ɉ�����Ă���L�[���
		int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);	// ��]��
		Input::Mouse::SetWheel((float)zDelta);
		break;
	}
	default:
		return DefWindowProc(hwnd, msg, wParam, lparam);
	}
	return 0;
}

bool CWindow::RegisterClass(HINSTANCE	 h_Instance,
	const char*	 h_ClassName,
	unsigned long h_Style)
{
	// �E�B���h�E�N���X�o�^
	WNDCLASSEX WndClassEx = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		h_Instance,
		nullptr,
		nullptr,
		(HBRUSH)(COLOR_WINDOW + 1),
		nullptr,
		h_ClassName,
		nullptr
	};
//	WndClassEx.cbSize = sizeof(WNDCLASSEX);
	WndClassEx.style = h_Style;
//	WndClassEx.lpfnWndProc = WndProc;
//	WndClassEx.cbClsExtra = 0L;
//	WndClassEx.cbWndExtra = 0L;
//	WndClassEx.hInstance = h_Instance;
//	WndClassEx.hIcon = nullptr;
//	WndClassEx.hCursor = nullptr;
////	WndClassEx.hCursor = LoadCursor(nullptr, IDC_ARROW);
////	WndClassEx.hbrBackground = nullptr;
//	WndClassEx.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
//	WndClassEx.lpszMenuName = nullptr;
//	WndClassEx.lpszClassName = h_ClassName;
//	WndClassEx.hIconSm = NULL;

	// COM������
	if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
	{
		MessageBox(NULL, "COM�̏������Ɏ��s���܂����B", "error", MB_OK);
		return false;
	}

	// �E�B���h�E�N���X�̓o�^
	if (!RegisterClassEx(&WndClassEx))
	{
		MessageBox(NULL, "RegisterClassEx", "Error!", MB_OK);
		return false;
	}

	return true;
}

void CWindow::SetWindow(HINSTANCE		h_Instance,
	unsigned long	h_Style,
	unsigned long	h_ExStyle,
	HWND				h_Parent,
	long				h_Width,
	long				h_Height,
	const char*		h_ClassName,
	const char*		h_Title,
	bool				FULLSCREEN)
{
	// �N���C�A���g�̈�T�C�Y����E�B���h�E �T�C�Y�Z�o
	DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION
		| WS_SYSMENU | WS_BORDER | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	DWORD dwExStyle = 0;
	RECT rc = { 0, 0, h_Width, h_Height };
	AdjustWindowRectEx(&rc, dwStyle, FALSE, dwExStyle);
	RECT	rWindow, rClient;

	if (FULLSCREEN) {
		m_hwnd = CreateWindowEx(
			dwExStyle,//h_ExStyle,
			h_ClassName,					// �E�B���h�E�N���X�̖��O
			h_Title,						// �^�C�g��
			dwStyle,//h_Style,						// �E�B���h�E�X�^�C��
			CW_USEDEFAULT, CW_USEDEFAULT,	// �E�B���h�E�ʒu �c, ��
			//h_Width, h_Height,				// �E�B���h�E�T�C�Y
			rc.right - rc.left,
			rc.bottom - rc.top,
			NULL,							// �e�E�B���h�E�Ȃ�
			(HMENU)NULL,					// ���j���[�Ȃ�
			h_Instance,						// �C���X�^���X�n���h��
			(LPVOID)NULL);					// �ǉ������Ȃ�
	}
	else {

		m_hwnd = CreateWindowEx(
			h_ExStyle,
			h_ClassName,					// �E�B���h�E�N���X�̖��O
			h_Title,						// �^�C�g��
			WS_CAPTION | WS_SYSMENU,		// �E�B���h�E�X�^�C��
			0, 0,							// �E�B���h�E�ʒu �c, ��(���ƂŒ����Ɉړ������܂�)
			h_Width, h_Height,				// �E�B���h�E�T�C�Y
			HWND_DESKTOP,					// �e�E�B���h�E�Ȃ�
			(HMENU)NULL,					// ���j���[�Ȃ�
			h_Instance,						// �C���X�^���X�n���h��
			(LPVOID)NULL);					// �ǉ������Ȃ�

	
	}
	{
		// �E�C���h�E�T�C�Y���Čv�Z�iMetrics�����ł́A�t���[���f�U�C���ŃN���C�A���g�̈�T�C�Y���ς���Ă��܂��̂Łj
		GetWindowRect(m_hwnd, &rWindow);
		GetClientRect(m_hwnd, &rClient);
		if (rClient.right != h_Width || rClient.bottom != h_Height)
		{
			int width = (rWindow.right - rWindow.left) - (rClient.right - rClient.left) + h_Width;
			int height = (rWindow.bottom - rWindow.top) - (rClient.bottom - rClient.top) + h_Height;

			SetWindowPos(
				m_hwnd,
				NULL,
				CW_USEDEFAULT,	// GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2,
				CW_USEDEFAULT,	// GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2,
				width,
				height,
				SWP_NOMOVE | SWP_NOZORDER | SWP_NOOWNERZORDER
			);
		}
	}

	if (!m_hwnd) {
		MessageBox(nullptr, "CreateWindow Error", "error", MB_OK);
	}
}
bool CWindow::ExecMessage()
{
	// ���b�Z�[�W�m�F
//  if( PeekMessage( &m_Message, NULL, 0, 0, PM_NOREMOVE ) )�@// ���܂��Ă��郁�b�Z�[�W��S�ď����@20210622
	while (PeekMessage(&m_Message, NULL, 0, 0, PM_NOREMOVE))
	{
		// ���b�Z�[�W�Ȃ�
		if (!(::GetMessage(&m_Message, NULL, 0, 0)))
			return false;

		TranslateMessage(&m_Message);	// �L�[���b�Z�[�W�̕ϊ�
		DispatchMessage(&m_Message);	// ���b�Z�[�W�̑��o
	}

	return true;
}


LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT ImGui_WndProc(HWND hwnd, 		// �E�B���h�E�n���h��
	UINT message,	// ���b�Z�[�W���ʎq
	WPARAM wParam,	// �t�я��P
	LPARAM lParam)	// �t�я��Q
{
	ImGui_ImplWin32_WndProcHandler(hwnd, message, wParam, lParam);
	/*if (ImGui_ImplWin32_WndProcHandler(hwnd, message, wParam, lParam))
		return true;*/

	switch (message) {

	case WM_KEYDOWN:						// �L�[�{�[�h����	
		switch (wParam) {
		case VK_ESCAPE:
			// �E�C���h�E���I��������
			DestroyWindow(hwnd);
			break;
		}
		break;
	case WM_DESTROY:						// �E�C���h�E�I����
		PostQuitMessage(0);
		break;
	default:
	return 0;
		//return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}

void CWindow::Close(const char* name, HINSTANCE inst)
{
}