//=========================================================
// [keyboad.h]
// �쐬:2022/06/26
//=========================================================

#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#define NOMINMAX

#include <windows.h>

namespace MySpace
{
	namespace Input
	{
#define	NUM_KEY_MAX			(256)	// �L�[�̍ő吔

		class Keyboad
		{
		public:
			static void Update();
			static void BeginUpdate();

			static bool IsPress(int nKey);
			static bool IsTrigger(int nKey);
			static bool IsRelease(int nKey);

		public:
			static inline WORD keyState[NUM_KEY_MAX];			// �L�[�{�[�h�̓��͏�񃏁[�N
			static inline WORD keyStateTrigger[NUM_KEY_MAX];	// �L�[�{�[�h�̃g���K�[��񃏁[�N
			static inline WORD keyStateRelease[NUM_KEY_MAX];	// �L�[�{�[�h�̃����[�X��񃏁[�N
		};
	}
}
#endif // !_KEYBOARD_H_
