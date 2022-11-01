//=========================================================
// [keyboad.cpp]
// �쐬:2022/06/26
//=========================================================

#include <CoreSystem/Input/Keyboard.h>


using namespace MySpace::Input;

//BYTE Keyboad::keyState[NUM_KEY_MAX];
//BYTE Keyboad::keyStateTrigger[NUM_KEY_MAX];
//BYTE Keyboad::keyStateRelease[NUM_KEY_MAX];

void Keyboad::Update()
{
	WORD wKeyState;

	// �f�o�C�X����f�[�^���擾
	//if (GetKeyboardState(wKeyState))
	{
		for (int nCnt = 0; nCnt < NUM_KEY_MAX; nCnt++)
		{
			wKeyState = GetAsyncKeyState(nCnt);

			// �L�[�g���K�[�E�����[�X���𐶐�
			keyStateTrigger[nCnt] = (keyState[nCnt] ^ wKeyState) & wKeyState;
			keyStateRelease[nCnt] = (keyState[nCnt] ^ wKeyState) & ~wKeyState;

			// �L�[�v���X����ۑ�
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
