//==========================================================
// [input.cpp]
// 2022/06/11 作成
//==========================================================

//--- インクルード部
#include <CoreSystem/Input/gamePad.h>

// 名前空間
using namespace MySpace::Input;

namespace {
	const float THUMB_VALUE = (65535 / 4);

}
//--- 静的メンバ変数
XINPUT_STATE CGamePad::m_XInputState;				//XBOXコントローラーの状態を格納する
XINPUT_VIBRATION CGamePad::m_Vibration;			//XBOXコントローラーのバイブレーション
bool CGamePad::g_bEnable = false;

BYTE CGamePad::m_PadState[(int)EButton::MAX];			// コントローラーの入力情報ワーク
BYTE CGamePad::m_PadTrigger[(int)EButton::MAX];	// コントローラーのトリガー情報ワーク
BYTE CGamePad::m_PadRelease[(int)EButton::MAX];	// コントローラーのリリース情報ワーク


CGamePad::CGamePad()
{
}
CGamePad::~CGamePad()
{
}
void CGamePad::Init()
{
	if(XInputGetState(0,&m_XInputState) == ERROR_SUCCESS)
	{ 
		g_bEnable = true;
	}
}
void CGamePad::Uninit()
{
	if (!g_bEnable) { return; }

	XInputEnable(false);
}
void CGamePad::Update()
{
	if (!g_bEnable)return;

	XInputGetState(0, &m_XInputState);

	BYTE aPadState[(int)EButton::MAX];
	
	{
		aPadState[(int)EButton::DPAD_UP]        = (m_XInputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) ? 1 : 0;
		aPadState[(int)EButton::DPAD_DOWN]      = (m_XInputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) ? 1 : 0;
		aPadState[(int)EButton::DPAD_LEFT]      = (m_XInputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) ? 1 : 0;
		aPadState[(int)EButton::DPAD_RIGHT]     = (m_XInputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) ? 1 : 0;
		aPadState[(int)EButton::START]          = (m_XInputState.Gamepad.wButtons & XINPUT_GAMEPAD_START) ? 1 : 0;
		aPadState[(int)EButton::BACK]           = (m_XInputState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) ? 1 : 0;
		aPadState[(int)EButton::LEFT_SHOULDER]  = (m_XInputState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) ? 1 : 0;
		aPadState[(int)EButton::RIGHT_SHOULDER] = (m_XInputState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) ? 1 : 0;
		aPadState[(int)EButton::A]              = (m_XInputState.Gamepad.wButtons & XINPUT_GAMEPAD_A) ? 1 : 0;
		aPadState[(int)EButton::B]              = (m_XInputState.Gamepad.wButtons & XINPUT_GAMEPAD_B) ? 1 : 0;
		aPadState[(int)EButton::X]              = (m_XInputState.Gamepad.wButtons & XINPUT_GAMEPAD_X) ? 1 : 0;
		aPadState[(int)EButton::Y]              = (m_XInputState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) ? 1 : 0;

		aPadState[(int)EButton::THUMB_L_UP]    = (m_XInputState.Gamepad.sThumbLY >= THUMB_VALUE) ? 1 : 0;
		aPadState[(int)EButton::THUMB_L_DOWN]  = (m_XInputState.Gamepad.sThumbLY <= -THUMB_VALUE) ? 1 : 0;
		aPadState[(int)EButton::THUMB_L_LEFT]  = (m_XInputState.Gamepad.sThumbLX <= -THUMB_VALUE) ? 1 : 0;
		aPadState[(int)EButton::THUMB_L_RIGHT] = (m_XInputState.Gamepad.sThumbLX >= THUMB_VALUE) ? 1 : 0;
		aPadState[(int)EButton::THUMB_R_UP]    = (m_XInputState.Gamepad.sThumbRY >= THUMB_VALUE) ? 1 : 0;
		aPadState[(int)EButton::THUMB_R_DOWN]  = (m_XInputState.Gamepad.sThumbRY <= -THUMB_VALUE) ? 1 : 0;
		aPadState[(int)EButton::THUMB_R_LEFT]  = (m_XInputState.Gamepad.sThumbRX <= -THUMB_VALUE) ? 1 : 0;
		aPadState[(int)EButton::THUMB_R_RIGHT] = (m_XInputState.Gamepad.sThumbRX >= THUMB_VALUE) ? 1 : 0;

		aPadState[(int)EButton::LEFT_TRIGGER]  = (m_XInputState.Gamepad.bLeftTrigger == 255) ? 1 : 0;
		aPadState[(int)EButton::RIGHT_TRIGGER] = (m_XInputState.Gamepad.bRightTrigger == 255) ? 1 : 0;
	}
	
	for (int nCnKey = 0; nCnKey < (int)EButton::MAX; nCnKey++)
	{
		// キートリガー・リリース情報を生成
		m_PadTrigger[nCnKey] = (m_PadState[nCnKey] ^ aPadState[nCnKey]) & aPadState[nCnKey];
		m_PadRelease[nCnKey] = (m_PadState[nCnKey] ^ aPadState[nCnKey]) & m_PadState[nCnKey];


		// キープレス情報を保存
		m_PadState[nCnKey] = aPadState[nCnKey];
	}
}

bool CGamePad::Enable()
{
	return g_bEnable;
}

bool CGamePad::IsPress(EButton nKey)
{
	if (!g_bEnable) { return false; }

	return (m_PadState[(int)nKey]) ? true : false;
}

bool CGamePad::IsTrigger(EButton nKey)
{
	if (!g_bEnable) { return false; }

	return (m_PadTrigger[(int)nKey]) ? true : false;
}

bool CGamePad::IsRelease(EButton nKey)
{
	if (!g_bEnable) { return false; }

	return (m_PadRelease[(int)nKey]) ? true : false;
}

float CGamePad::ThumbLX()
{
	if (!g_bEnable) { return 0.0f; }

	if (m_XInputState.Gamepad.sThumbLX < 0)
	{
		return (float)m_XInputState.Gamepad.sThumbLX / 32768.0f;
	}
	else
	{
		return (float)m_XInputState.Gamepad.sThumbLX / 32767.0f;
	}
}

float CGamePad::ThumbLY()
{
	if (!g_bEnable) { return 0.0f; }

	if (m_XInputState.Gamepad.sThumbLY < 0)
	{
		return (float)m_XInputState.Gamepad.sThumbLY / 32768.0f;
	}
	else
	{
		return (float)m_XInputState.Gamepad.sThumbLY / 32767.0f;
	}
}

float CGamePad::ThumbRX()
{
	if (!g_bEnable) { return 0.0f; }

	if (m_XInputState.Gamepad.sThumbRX < 0)
	{
		return (float)m_XInputState.Gamepad.sThumbRX / 32768.0f;
	}
	else
	{
		return (float)m_XInputState.Gamepad.sThumbRX / 32767.0f;
	}
}

float CGamePad::ThumbRY()
{
	if (!g_bEnable) { return 0.0f; }

	if (m_XInputState.Gamepad.sThumbRY < 0)
	{
		return (float)m_XInputState.Gamepad.sThumbRY / 32768.0f;
	}
	else
	{
		return (float)m_XInputState.Gamepad.sThumbRY / 32767.0f;
	}
}
