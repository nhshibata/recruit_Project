//==========================================================
// [input.cpp]
// 2022/06/11 作成
//==========================================================

//--- インクルード部
#include <CoreSystem/Input/input.h>
#include <Application/Application.h>

using namespace MySpace::System;

namespace 
{
	const int	g_nMouseBtn[] = { VK_LBUTTON, VK_RBUTTON, VK_MBUTTON, VK_XBUTTON1, VK_XBUTTON2 };
};

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
WORD		CInput::m_wKeyState[NUM_KEY_MAX];			// キーボードの押下状態を保持するワーク
WORD		CInput::m_wKeyStateTrigger[NUM_KEY_MAX];	// キーボードのトリガ状態を保持するワーク
WORD		CInput::m_wKeyStateRelease[NUM_KEY_MAX];	// キーボードのリリース状態を保持するワーク
WORD		CInput::m_wKeyStateRepeat[NUM_KEY_MAX];		// キーボードのリピート状態を保持するワーク
int			CInput::m_nKeyStateRepeatCnt[NUM_KEY_MAX];	// キーボードのリピートカウンタ

DWORD		CInput::m_dwJoyCount;
JOYINFOEX	CInput::m_joyState[NUM_JOY_MAX];
DWORD		CInput::m_dwJoyButtonTrigger[NUM_JOY_MAX];
DWORD		CInput::m_dwJoyButtonRelease[NUM_JOY_MAX];

POINT		CInput::m_ptMouse;


//=============================================================================
// 入力処理の初期化
//=============================================================================
HRESULT CInput::Init()
{
	HRESULT hr = S_OK;

	// キーボードの初期化
	hr = InitKeyboard();

	// ゲームパッドの初期化
	if (SUCCEEDED(hr)) {
		hr = InitJoystick();
	}

	// マウスの初期化
	if (SUCCEEDED(hr)) {
		hr = InitMouse();
	}

	return hr;
}

//=============================================================================
// 入力処理の終了処理
//=============================================================================
void CInput::Fin()
{
	// マウスの終了処理
	UninitMouse();

	// ゲームパッドの終了処理
	UninitJoystick();

	// キーボードの終了処理
	UninitKeyboard();
}

//=============================================================================
// 入力処理の更新処理
//=============================================================================
void CInput::Update()
{
	// キーボードの更新
	UpdateKeyboard();

	// ゲームパッドの更新
	UpdateJoystick();

	// マウスの更新
	UpdateMouse();
}

//=============================================================================
// キーボードの初期化処理
//=============================================================================
HRESULT CInput::InitKeyboard()
{
	HRESULT hr = S_OK;

	return hr;
}

//=============================================================================
// キーボードの終了処理
//=============================================================================
void CInput::UninitKeyboard(void)
{
	// (何もしない)
}

//=============================================================================
// キーボードの更新処理
//=============================================================================
HRESULT CInput::UpdateKeyboard(void)
{
	HRESULT hr = S_OK;
	WORD wKeyState;

	// キーボードの状態を取得
	for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++) {
		wKeyState = GetAsyncKeyState(nCntKey);

		m_wKeyStateTrigger[nCntKey] = (m_wKeyState[nCntKey] ^ wKeyState) & wKeyState;
		m_wKeyStateRelease[nCntKey] = (m_wKeyState[nCntKey] ^ wKeyState) & ~wKeyState;
		m_wKeyStateRepeat[nCntKey] = m_wKeyStateTrigger[nCntKey];

		if (wKeyState & 0x8000) {
			m_nKeyStateRepeatCnt[nCntKey]++;
			if (m_nKeyStateRepeatCnt[nCntKey] >= 20) {
				m_wKeyStateRepeat[nCntKey] = wKeyState;
			}
		}
		else {
			m_nKeyStateRepeatCnt[nCntKey] = 0;
			m_wKeyStateRepeat[nCntKey] = 0;
		}

		m_wKeyState[nCntKey] = wKeyState;
	}

	return hr;
}

//=============================================================================
// ゲームパッドの初期化処理
//=============================================================================
HRESULT CInput::InitJoystick()
{
	GetJoyCount();

	return S_OK;
}

//=============================================================================
// ゲームパッドの終了処理
//=============================================================================
void CInput::UninitJoystick()
{
	// (何もしない)
}

//=============================================================================
// ゲームパッドの更新処理
//=============================================================================
HRESULT CInput::UpdateJoystick()
{
	HRESULT hr = S_OK;

	for (DWORD i = 0; i < m_dwJoyCount; ++i) {
		JOYINFOEX& ji = m_joyState[i];
		DWORD dwButtons = ji.dwButtons;
		ZeroMemory(&ji, sizeof(ji));
		ji.dwSize = sizeof(ji);
		ji.dwFlags = JOY_RETURNALL;
		if (joyGetPosEx(i, &ji) != JOYERR_NOERROR) {
			hr = E_FAIL;
			break;
		}
		m_dwJoyButtonTrigger[i] = (m_joyState[i].dwButtons ^ dwButtons) & dwButtons;
		m_dwJoyButtonRelease[i] = (m_joyState[i].dwButtons ^ dwButtons) & ~dwButtons;
	}
	return hr;
}

//=============================================================================
// マウスの初期化処理
//=============================================================================
HRESULT CInput::InitMouse()
{
	return S_OK;
}

//=============================================================================
// マウスの終了処理
//=============================================================================
void CInput::UninitMouse()
{
	// (何もしない)
}

//=============================================================================
// マウスの更新処理
//=============================================================================
HRESULT CInput::UpdateMouse()
{
	//extern HWND g_hWnd;	// main.cpp
	HWND g_hWnd = Application::Get()->GetHWnd();	// main.cpp
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	m_ptMouse = pt;
	return S_OK;
}

//=============================================================================
// キーボードのプレス状態を取得
//=============================================================================
bool CInput::GetKeyPress(int nKey)
{
	return (m_wKeyState[nKey] & 0x8000) ? true : false;
}

//=============================================================================
// キーボードのトリガ状態を取得
//=============================================================================
bool CInput::GetKeyTrigger(int nKey)
{
	return (m_wKeyStateTrigger[nKey] & 0x8000) ? true : false;
}

//=============================================================================
// キーボードのリピート状態を取得
//=============================================================================
bool CInput::GetKeyRepeat(int nKey)
{
	return (m_wKeyStateRepeat[nKey] & 0x8000) ? true : false;
}

//=============================================================================
// キーボードのリリ－ス状態を取得
//=============================================================================
bool CInput::GetKeyRelease(int nKey)
{
	return (m_wKeyStateRelease[nKey] & 0x8000) ? true : false;
}

//=============================================================================
// ゲームパッド接続数取得
//=============================================================================
DWORD CInput::GetJoyCount()
{
	JOYINFO	ji;
	DWORD dwCount = joyGetNumDevs();
	if (dwCount > NUM_JOY_MAX)
		dwCount = NUM_JOY_MAX;
	for (m_dwJoyCount = 0; m_dwJoyCount < dwCount; ++m_dwJoyCount) {
		if (joyGetPos(m_dwJoyCount, &ji) != JOYERR_NOERROR)
			break;
	}
	return m_dwJoyCount;
}

//=============================================================================
// ゲームパッド情報取得
//=============================================================================
JOYINFOEX* CInput::GetJoyState(DWORD dwJoy)
{
	if (dwJoy >= NUM_JOY_MAX) return nullptr;
	return &m_joyState[dwJoy];
}

//=============================================================================
// ゲームパッドX軸取得
//=============================================================================
LONG CInput::GetJoyX(DWORD dwJoy)
{
	if (dwJoy >= NUM_JOY_MAX) return 0L;
	return (LONG)m_joyState[dwJoy].dwXpos - 0x08000;
}

//=============================================================================
// ゲームパッドY軸取得
//=============================================================================
LONG CInput::GetJoyY(DWORD dwJoy)
{
	if (dwJoy >= NUM_JOY_MAX) return 0L;
	return (LONG)m_joyState[dwJoy].dwYpos - 0x08000;
}

//=============================================================================
// ゲームパッドZ軸取得
//=============================================================================
LONG CInput::GetJoyZ(DWORD dwJoy)
{
	if (dwJoy >= NUM_JOY_MAX) return 0L;
	return (LONG)m_joyState[dwJoy].dwZpos - 0x08000;
}

//=============================================================================
// ゲームパッド ボタン情報取得
//=============================================================================
bool CInput::GetJoyButton(DWORD dwJoy, DWORD dwBtn)
{
	if (dwJoy >= NUM_JOY_MAX) return false;
	return (m_joyState[dwJoy].dwButtons & (1 << dwBtn)) ? true : false;
}

//=============================================================================
// ゲームパッド トリガ情報取得
//=============================================================================
bool CInput::GetJoyTrigger(DWORD dwJoy, DWORD dwBtn)
{
	if (dwJoy >= NUM_JOY_MAX) return false;
	return (m_dwJoyButtonTrigger[dwJoy] & (1 << dwBtn)) ? true : false;
}

//=============================================================================
// ゲームパッド リリース情報取得
//=============================================================================
bool CInput::GetJoyRelease(DWORD dwJoy, DWORD dwBtn)
{
	if (dwJoy >= NUM_JOY_MAX) return false;
	return (m_dwJoyButtonRelease[dwJoy] & (1 << dwBtn)) ? true : false;
}

//=============================================================================
// マウス座標取得
//=============================================================================
POINT* CInput::GetMousePosition()
{
	return &m_ptMouse;
}

//=============================================================================
// マウス ボタン情報取得
//=============================================================================
bool CInput::GetMouseButton(DWORD dwBtn)
{
	if (dwBtn >= _countof(g_nMouseBtn)) return false;
	return GetKeyPress(g_nMouseBtn[dwBtn]);
}

//=============================================================================
// マウス トリガ情報取得
//=============================================================================
bool CInput::GetMouseTrigger(DWORD dwBtn)
{
	if (dwBtn >= _countof(g_nMouseBtn)) return false;
	return GetKeyTrigger(g_nMouseBtn[dwBtn]);
}

//=============================================================================
// マウス リリース情報取得
//=============================================================================
bool CInput::GetMouseRelease(DWORD dwBtn)
{
	if (dwBtn >= _countof(g_nMouseBtn)) return false;
	return GetKeyRelease(g_nMouseBtn[dwBtn]);
}