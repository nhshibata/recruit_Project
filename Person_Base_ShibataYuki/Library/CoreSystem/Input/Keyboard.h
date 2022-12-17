//=========================================================
// [keyboad.h]
// 作成:2022/06/26
//=========================================================

#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#define NOMINMAX

//--- インクルード部
#include <windows.h>

#define	NUM_KEY_MAX			(256)	// キーの最大数

namespace MySpace
{
	namespace Input
	{
		class Keyboad
		{
		public:
			//--- メンバ変数
			static inline WORD keyState[NUM_KEY_MAX];			// キーボードの入力情報ワーク
			static inline WORD keyStateTrigger[NUM_KEY_MAX];	// キーボードのトリガー情報ワーク
			static inline WORD keyStateRelease[NUM_KEY_MAX];	// キーボードのリリース情報ワーク
		
		public:
			//--- メンバ関数
			static void Update();
			static void BeginUpdate();

			static bool IsPress(int nKey);
			static bool IsTrigger(int nKey);
			static bool IsRelease(int nKey);

		};
	}
}
#endif // !_KEYBOARD_H_
