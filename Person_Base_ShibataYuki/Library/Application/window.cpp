//=========================================================
// [window.cpp] 
// 作成:2022/05/24
//---------------------------------------------------------
//=========================================================

#include <Application/window.h>
#include <CoreSystem/Input/mouse.h>
#include <ImGui/imgui.h>

using namespace MySpace;
using namespace MySpace::System;

//--- 定数定義

//--- 静的メンバ変数

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT ImGui_WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lparam)
{
	
	// コールバックが設定されていなければ終了
	ImGui_WndProc(hwnd, msg, wParam, lparam);
	
	// メッセージ処理
	switch (msg)
	{
		// キー押下
	case WM_KEYDOWN:
		// GM
		switch (wParam) {
		case VK_ESCAPE:					// [ESC]キーが押された
			PostMessage(hwnd, WM_CLOSE, 0, 0);	// [x]が押されたように振舞う
			DestroyWindow(hwnd);
			return DefWindowProc(hwnd, msg, wParam, lparam);
		}
		return DefWindowProc(hwnd, msg, wParam, lparam);

		// キー解放
	case WM_KEYUP:
		return DefWindowProc(hwnd, msg, wParam, lparam);

		// システムキー押下
	case WM_SYSKEYDOWN:
		return DefWindowProc(hwnd, msg, wParam, lparam);

		// システムキー解放
	case WM_SYSKEYUP:
		return DefWindowProc(hwnd, msg, wParam, lparam);

		// ×
	case WM_CLOSE:
		PostQuitMessage(0);
		return DefWindowProc(hwnd, msg, wParam, lparam);

		// ウインドウ解放
	case WM_DESTROY:
		PostQuitMessage(0);
		return DefWindowProc(hwnd, msg, wParam, lparam);

		// 再描画
	case WM_PAINT:
		return DefWindowProc(hwnd, msg, wParam, lparam);

		//=========================================================
	case WM_LBUTTONDOWN:
	{
		SetCapture(hwnd);	// カーソルが外に出ても反応するように設定
		Input::Mouse::SetButtonDown(Input::Mouse::EButton::Left);
		break;
	}
	case WM_LBUTTONUP:
	{
		ReleaseCapture();	// カーソルが外に出ても反応しないように設定
		Input::Mouse::SetButtonUp(Input::Mouse::EButton::Left);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		SetCapture(hwnd);	// カーソルが外に出ても反応するように設定
		Input::Mouse::SetButtonDown(Input::Mouse::EButton::Right);
		break;
	}
	case WM_RBUTTONUP:
	{
		ReleaseCapture();	// カーソルが外に出ても反応しないように設定
		Input::Mouse::SetButtonUp(Input::Mouse::EButton::Right);
		break;
	}
	case WM_MBUTTONDOWN:
	{
		SetCapture(hwnd);	// カーソルが外に出ても反応するように設定
		Input::Mouse::SetButtonDown(Input::Mouse::EButton::Middle);
		break;
	}
	case WM_MBUTTONUP:
	{
		ReleaseCapture();	// カーソルが外に出ても反応しないように設定
		Input::Mouse::SetButtonUp(Input::Mouse::EButton::Middle);
		break;
	}
	case WM_MOUSEWHEEL:
	{
		//DWORD fwKeys = GET_KEYSTATE_WPARAM(wParam);	// 同時に押されているキー情報
		int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);	// 回転量
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
	// ウィンドウクラス登録
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

	// COM初期化
	if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
	{
		MessageBox(NULL, "COMの初期化に失敗しました。", "error", MB_OK);
		return false;
	}

	// ウィンドウクラスの登録
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
	// クライアント領域サイズからウィンドウ サイズ算出
	DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION
		| WS_SYSMENU | WS_BORDER | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	DWORD dwExStyle = 0;
	RECT rc = { 0, 0, h_Width, h_Height };
	AdjustWindowRectEx(&rc, dwStyle, FALSE, dwExStyle);
	RECT	rWindow, rClient;

	if (FULLSCREEN) {
		m_hwnd = CreateWindowEx(
			dwExStyle,//h_ExStyle,
			h_ClassName,					// ウィンドウクラスの名前
			h_Title,						// タイトル
			dwStyle,//h_Style,						// ウィンドウスタイル
			CW_USEDEFAULT, CW_USEDEFAULT,	// ウィンドウ位置 縦, 横
			//h_Width, h_Height,				// ウィンドウサイズ
			rc.right - rc.left,
			rc.bottom - rc.top,
			NULL,							// 親ウィンドウなし
			(HMENU)NULL,					// メニューなし
			h_Instance,						// インスタンスハンドル
			(LPVOID)NULL);					// 追加引数なし
	}
	else {

		m_hwnd = CreateWindowEx(
			h_ExStyle,
			h_ClassName,					// ウィンドウクラスの名前
			h_Title,						// タイトル
			WS_CAPTION | WS_SYSMENU,		// ウィンドウスタイル
			0, 0,							// ウィンドウ位置 縦, 横(あとで中央に移動させます)
			h_Width, h_Height,				// ウィンドウサイズ
			HWND_DESKTOP,					// 親ウィンドウなし
			(HMENU)NULL,					// メニューなし
			h_Instance,						// インスタンスハンドル
			(LPVOID)NULL);					// 追加引数なし

	
	}
	{
		// ウインドウサイズを再計算（Metricsだけでは、フレームデザインでクライアント領域サイズが変わってしまうので）
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
	// メッセージ確認
//  if( PeekMessage( &m_Message, NULL, 0, 0, PM_NOREMOVE ) )　// たまっているメッセージを全て処理　20210622
	while (PeekMessage(&m_Message, NULL, 0, 0, PM_NOREMOVE))
	{
		// メッセージなし
		if (!(::GetMessage(&m_Message, NULL, 0, 0)))
			return false;

		TranslateMessage(&m_Message);	// キーメッセージの変換
		DispatchMessage(&m_Message);	// メッセージの送出
	}

	return true;
}


LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT ImGui_WndProc(HWND hwnd, 		// ウィンドウハンドル
	UINT message,	// メッセージ識別子
	WPARAM wParam,	// 付帯情報１
	LPARAM lParam)	// 付帯情報２
{
	ImGui_ImplWin32_WndProcHandler(hwnd, message, wParam, lParam);
	/*if (ImGui_ImplWin32_WndProcHandler(hwnd, message, wParam, lParam))
		return true;*/

	switch (message) {

	case WM_KEYDOWN:						// キーボード押下	
		switch (wParam) {
		case VK_ESCAPE:
			// ウインドウを終了させる
			DestroyWindow(hwnd);
			break;
		}
		break;
	case WM_DESTROY:						// ウインドウ終了時
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