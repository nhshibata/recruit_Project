//=========================================================
// [keyboad.cpp]
// 作成:2022/06/26
//=========================================================

#include <CoreSystem/Input/Keyboard.h>


using namespace MySpace::Input;

//BYTE Keyboad::keyState[NUM_KEY_MAX];
//BYTE Keyboad::keyStateTrigger[NUM_KEY_MAX];
//BYTE Keyboad::keyStateRelease[NUM_KEY_MAX];

void Keyboad::Update()
{
	WORD wKeyState;

	// デバイスからデータを取得
	//if (GetKeyboardState(wKeyState))
	{
		for (int nCnt = 0; nCnt < NUM_KEY_MAX; nCnt++)
		{
			wKeyState = GetAsyncKeyState(nCnt);

			// キートリガー・リリース情報を生成
			keyStateTrigger[nCnt] = (keyState[nCnt] ^ wKeyState) & wKeyState;
			keyStateRelease[nCnt] = (keyState[nCnt] ^ wKeyState) & ~wKeyState;

			// キープレス情報を保存
			keyState[nCnt] = wKeyState;
		}
	}
}
void Keyboad::BeginUpdate()
{
	for (int nCnt = 0; nCnt < NUM_KEY_MAX; nCnt++)
	{
		if(IsRelease(nCnt))
			keyState[nCnt] = false;
	}
}
bool Keyboad::IsPress(int nKey)
{
	//return (keyState[nKey] & 0x80) != 0;
	return (keyState[nKey] & 0x8000) ? true : false;
}

bool Keyboad::IsTrigger(int nKey)
{
	return (keyStateTrigger[nKey] & 0x8000) ? true : false;
}

bool Keyboad::IsRelease(int nKey)
{
	return (keyStateRelease[nKey] & 0x8000) ? true : false;
}
