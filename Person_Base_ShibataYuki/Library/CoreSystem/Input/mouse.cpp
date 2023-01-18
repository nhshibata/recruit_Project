//=========================================================
// [mouse.cpp]
// 作成:2022/06/26
//=========================================================

//--- インクルード部
#include <CoreSystem/Input/mouse.h>
#include <Application/Application.h>

using namespace MySpace;
using namespace MySpace::Input;
using namespace MySpace::System;


void Mouse::Initialize()
{
	POINT pos, pos1, pos2;
	pos = pos1 = pos2 = {0,0};
	
	// ウィンドウの左上の位置を取得 ＆ ウィンドウのマウス座標取得
	if (ScreenToClient(Application::Get()->GetHWnd(), &pos1) &&
		GetCursorPos(&pos2))
	{
		pos.x = pos1.x + pos2.x + GetSystemMetrics(SM_CXDLGFRAME) * 2;
		pos.y = pos1.y + pos2.y + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION);
		m_vCursorPos = Vector2((float)pos.x, (float)pos.y);
		m_vPrevCursorPos = m_vCursorPos;
		m_vVel = Vector2(0,0);
	}
}

void Mouse::Update()
{
	if (!m_bWheel)
		m_fWheel = 0.0f;
	m_bWheel = false;

	mouseStateTrigger[(int)EButton::Left] = (mouseState[(int)EButton::Left] ^ curState[(int)EButton::Left]) & curState[(int)EButton::Left];
	mouseStateTrigger[(int)EButton::Right] = (mouseState[(int)EButton::Right] ^ curState[(int)EButton::Right]) & curState[(int)EButton::Right];
	mouseStateTrigger[(int)EButton::Middle] = (mouseState[(int)EButton::Middle] ^ curState[(int)EButton::Middle]) & curState[(int)EButton::Middle];

	mouseStateRelease[(int)EButton::Left] = (mouseState[(int)EButton::Left] ^ curState[(int)EButton::Left]) & mouseState[(int)EButton::Left];
	mouseStateRelease[(int)EButton::Right] = (mouseState[(int)EButton::Right] ^ curState[(int)EButton::Right]) & mouseState[(int)EButton::Right];
	mouseStateRelease[(int)EButton::Middle] = (mouseState[(int)EButton::Middle] ^ curState[(int)EButton::Middle]) & mouseState[(int)EButton::Middle];

	mouseState[(int)EButton::Left] = curState[(int)EButton::Left];
	mouseState[(int)EButton::Right] = curState[(int)EButton::Right];
	mouseState[(int)EButton::Middle] = curState[(int)EButton::Middle];

	POINT pos;
	// ウィンドウからのマウス座標取得
	if (GetCursorPos(&pos)
		/*&& ScreenToClient(RendererSystem::hWnd, &pos)*/)
	{
		/*int windowFrameW = GetSystemMetrics(SM_CXDLGFRAME) / 2;
		int windowFrameH = windowFrameW + GetSystemMetrics(SM_CYCAPTION);
		pos.x = pos.x + windowFrameW * 4;
		pos.y = pos.y + windowFrameW * 4;*/

		// 何か押している時カーソル座標がウィンドウ外に出たらループする処理
		if (IsPressAny())
		{
			RECT wRect;
			//GetClientRect(RendererSystem::hWnd, &wRect);
			// PCのディスプレイサイズ取得
			HWND hDesktop = GetDesktopWindow();
			GetWindowRect(hDesktop, &wRect);

			LONG offset = 5;
			bool isOut = false;

			wRect.right -= 1;
			wRect.bottom -= 1;

			if (pos.x <= wRect.left) {
				pos.x = wRect.right - offset; isOut = true;
			}
			if (pos.x >= wRect.right) {
				pos.x = wRect.left + offset; isOut = true;
			}
			if (pos.y <= wRect.top) {
				pos.y = wRect.bottom - offset; isOut = true;
			}
			if (pos.y >= wRect.bottom) {
				pos.y = wRect.top + offset; isOut = true;
			}
		}

		m_vPrevCursorPos = m_vCursorPos;
		m_vCursorPos = Vector2((float)pos.x, (float)pos.y);
		m_vVel = m_vCursorPos - m_vPrevCursorPos;
	}
}

void Mouse::SetButtonDown(EButton button)
{
	curState[(int)button] = true;
}

void Mouse::SetButtonUp(EButton button)
{
	curState[(int)button] = false;
}

void Mouse::SetWheel(float value)
{
	m_fWheel = value;
	m_bWheel = true;
}

void Mouse::GetPosition(Vector2 * pos)
{
	*pos = m_vCursorPos;
}

Vector2 & Mouse::GetPosition()
{
	return m_vCursorPos;
}

bool Mouse::IsPress(EButton EButton)
{
	return mouseState[(int)EButton];
}

bool Mouse::IsPressAny()
{
	for (int i = 0; i < (int)EButton::MAX; i++)
	{
		if (mouseState[i]) return true;
	}
	return false;
}

bool Mouse::IsTriggerAny()
{
	for (int i = 0; i < (int)EButton::MAX; i++)
	{
		if (mouseStateTrigger[i]) return true;
	}
	return false;
}

bool Mouse::IsTrigger(EButton EButton)
{
	return mouseStateTrigger[(int)EButton];
}

bool Mouse::IsRelease(EButton EButton)
{
	return mouseStateRelease[(int)EButton];
}

void Mouse::GetMove(Vector2 * move)
{
	*move = m_vVel;
}

Vector2 & Mouse::GetMove()
{
	return m_vVel;
}

float Mouse::GetHwheel()
{
	return m_fWheel;
}
